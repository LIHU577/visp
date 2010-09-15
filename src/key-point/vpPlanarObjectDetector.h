/****************************************************************************
 *
 * $Id: $
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2010 by INRIA. All rights reserved.
 * 
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional 
 * Edition License.
 *
 * See http://www.irisa.fr/lagadic/visp/visp.html for more information.
 * 
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * Description:
 * Planar surface detection tool.
 *
 * Authors:
 * Romain Tallonneau
 *
 *****************************************************************************/

#ifndef VPPLANAROBJECTDETECTOR_H_
#define VPPLANAROBJECTDETECTOR_H_


#include <visp/vpConfig.h>

#if (VISP_HAVE_OPENCV_VERSION >= 0x020000) // Require opencv >= 2.0.0

#if (VISP_HAVE_OPENCV_VERSION >= 0x020101) // Require opencv >= 2.1.1
#  include <opencv2/imgproc/imgproc.hpp>
#  include <opencv2/features2d/features2d.hpp>
#  include <opencv2/calib3d/calib3d.hpp>
#elif (VISP_HAVE_OPENCV_VERSION >= 0x020000) // Require opencv >= 2.0.0
#  include <cv.h>
#  include <cvaux.hpp>
#endif

#include <visp/vpList.h>
#include <visp/vpImagePoint.h>
#include <visp/vpImage.h>
#include <visp/vpRect.h>
#include <visp/vpImagePoint.h>
#include <visp/vpPoint.h>
#include <visp/vpList.h>
#include <visp/vpHomogeneousMatrix.h>
#include <visp/vpCameraParameters.h>
#include <visp/vpHomography.h>
#include <visp/vpFernClassifier.h>

/*!
  \class vpPlanarObjectDetector
  \ingroup PlanarSurfaceDetector
  \brief Class used to detect a planar surface. 
  
  This class allows to learn and recognise a surface in an image based on the 
  Fern Classifier or any other point of interest matching class.
  
  It uses the class vpFernClassifier to extract points of interest in a 
  reference image. These points are recorded and a classifier is trained to 
  recognises them. 
  
  In this class the points detected are assumed to belong to a planar surface. 
  Therefore an homography can be computed between the reference image and the 
  current image if the object is detected in the image. 
  
  A robust method (RANSAC) is used to remove outliers in the matching process. 
  
  The following example shows how to use the class.
  
  \code 
#include <visp/vpConfig.h>
#include <visp/vpImage.h>
#include <visp/vpDisplay.h>
#include <visp/vpPlanarObjectDetector.h>

#if VISP_HAVE_OPENCV_VERSION >= 0x020000 // Surf Fern classifier only available since 2.1.0
int main()
{
  vpImage<unsigned char> Ireference;
  vpImage<unsigned char> Icurrent;
  vpPlanarObjectDetector planar;

  //First grab the reference image Irefrence

  //Select a part of the image by clincking on two points which define a rectangle
  vpImagePoint corners[2];
  for (int i=0 ; i < 2 ; i++)
  {
    vpDisplay::getClick(Ireference, corners[i]);
  }

  //Build the reference points (and train the classifier).
  int nbrRef;
  unsigned int height, width;
  height = (unsigned int)(corners[1].get_i() - corners[0].get_i());
  width = (unsigned int)(corners[1].get_j() - corners[0].get_j());
  nbrRef = planar.buildReference(Ireference, corners[0], height, width);

  //Then grab another image which represents the current image Icurrent

  //Match points between the reference points and the current points computed in the current image.
  bool isDetected;
  height = (unsigned int)(corners[1].get_i() - corners[0].get_i());
  width = (unsigned int)(corners[1].get_j() - corners[0].get_j());
  isDetected = planar.matchPoint(Icurrent, corners[0], height, width);

  //Display the matched points
  if(isDetected){
    planar.display(Ireference, Icurrent);
    vpHomography homography;
    planar.getHomography(homography);
  }
  else{
    std::cerr << "planar surface not detected in the current image" << std::endl;
  }

  return(0);
}
#else
int main() {}
#endif
  \endcode
    
*/  
class VISP_EXPORT vpPlanarObjectDetector{
protected:  
  //! Fern Classifier used to match the points between a reference image and the current image.
  vpFernClassifier fern;
  
  //! Computed homography in the ViSP format.
  vpHomography homography;
  //! Computed homography in the OpenCV format.
  cv::Mat H;
  
  //! The estimated new coordinates of the corners (reprojected using the homography).
  std::vector<cv::Point2f> dst_corners; 

  //! Flag to indicate wether the last computed homography is correct or not.
  bool isCorrect;
  
  //! Minimum number of point below which the homography is not considered as good.
  unsigned int nbMinPoint;
  
  //! The corners in the reference image
  std::vector<cv::Point2f> ref_corners;
  
  //! The ROI in the reference image.
  cv::Rect ref_ROI; 
  //! The ROI for the reference image.
  cv::Rect cur_ROI;
  //! The ROI for the reference image. 
  cv::Rect modelROI;

public:

    // constructors and destructors
  vpPlanarObjectDetector();
  vpPlanarObjectDetector(const std::string& dataFile, const std::string& objectName);
  virtual ~vpPlanarObjectDetector();

    // main functions
      // creation of reference
  int buildReference(const vpImage<unsigned char> &I);
  int buildReference(const vpImage<unsigned char> &I,
		       vpImagePoint &iP,
		       unsigned int height, unsigned int width);
  int buildReference(const vpImage<unsigned char> &I,
		       const vpRect rectangle);
    
    // matching
  bool matchPoint(const vpImage<unsigned char> &I);
  bool matchPoint(const vpImage<unsigned char> &I,
		   vpImagePoint &iP, unsigned int height, unsigned int width);
  bool matchPoint(const vpImage<unsigned char> &I, const vpRect rectangle);
    // database management
  void recordDetector(const std::string& objectName, const std::string& dataFile);
  void load(const std::string& dataFilename, const std::string& objName);
    
    
    // display
  void display(vpImage<unsigned char> &I, bool displayKpts = true);
  void display(vpImage<unsigned char> &Iref,
		 vpImage<unsigned char> &Icurrent);

  void setRoi(const vpImagePoint& tl, const vpImagePoint& br);
  
  /*!
    Return the positions of the detected corners.
    
    \return a vector of vpImagePoint containing the position of the corners of 
    the planar surface in the current image.
  */
  std::vector<vpImagePoint> getDetectedCorners() const;
  
  /*!
    Return a reference to the classifier.
    
    \return The fern classifier.
  */
  vpFernClassifier& getFernClassifier() {return this->fern;}  
  
  /*!
    Return the computed homography between the reference image and the current 
    image.
    
    \param H : The computed homography.
  */
  inline void getHomography(vpHomography& _H) const { _H = this->homography;}
  
  /*!
    get the minimum number of point to accept the computed homography.
    
    \return the minimum of point.
  */
  inline unsigned int getNbMinPoint() const { return this->nbMinPoint; }

  /*!
    Set the minimum number of point to validate the computed homography.
    
    \param nb : the new minimum number of point.
  */
  inline void setNbMinPoint(const unsigned int nb) { this->nbMinPoint = nb;}
    
protected:

  virtual void init();
  void computeRoi(vpImagePoint* ip, const int nbpt);
  void initialiseRefCorners(const cv::Rect& _modelROI);
};

#endif

#endif /* VPPLANAROBJECTDETECTOR_H_ */
