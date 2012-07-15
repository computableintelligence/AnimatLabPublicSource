# Install script for directory: C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/examples

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/OpenSceneGraph")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osg2cpp/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osganimate/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgautocapture/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgautotransform/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgbillboard/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgblendequation/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgcallback/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgcamera/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgcatch/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgclip/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgcompositeviewer/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgcopy/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgcubemap/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgdelaunay/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgcluster/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgdepthpartition/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgdepthpeeling/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgdrawinstanced/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgdistortion/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgfadetext/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgfont/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgforest/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgfxbrowser/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osggameoflife/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osggeodemo/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osggeometry/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osggeometryshaders/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osghangglide/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osghud/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgimagesequence/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgimpostor/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgintersection/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgkdtree/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgkeyboard/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgkeyboardmouse/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osglauncher/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osglight/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osglightpoint/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osglogicop/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osglogo/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgmanipulator/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgmotionblur/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgmovie/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgmultiplerendertargets/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgmultitexture/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgmultitexturecontrol/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgoccluder/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgocclusionquery/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgpackeddepthstencil/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgpagedlod/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgparametric/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgparticle/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgparticleeffects/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgphotoalbum/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgpick/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgplanets/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgpoints/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgpointsprite/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgprecipitation/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgprerender/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgprerendercubemap/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgreflect/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgrobot/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgscalarbar/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgscreencapture/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgscribe/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgsequence/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgshaders/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgshaderterrain/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgshadow/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgshape/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgsharedarray/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgsimplifier/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgsimulation/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgsidebyside/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgslice/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgspacewarp/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgspheresegment/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgspotlight/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgstereoimage/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgstereomatch/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgteapot/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgterrain/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgtessellate/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgtext/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgtext3D/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgtexture1D/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgtexture2D/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgtexture3D/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgtexturerectangle/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgthirdpersonview/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgvertexprogram/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgvolume/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwindows/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osganimationhardware/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osganimationtimeline/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osganimationnode/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osganimationmakepath/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osganimationskinning/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osganimationsolid/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osganimationviewer/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgbrowser/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetaddremove/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetbox/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetcanvas/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetframe/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetinput/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetlabel/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetmessagebox/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetmenu/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetnotebook/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetperformance/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetscrolled/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetshader/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetstyled/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgettable/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgwidgetwindow/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgunittests/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgmemorytest/cmake_install.cmake")
  INCLUDE("C:/Projects/AnimatLabSDK/AnimatLabPublicSource/Libraries/OpenSceneGraph-2.8.3/OsgBuild/examples/osgpdf/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

