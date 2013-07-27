#pragma once

namespace OsgAnimatSim
{
	namespace Visualization
	{

		class OsgTrackballDragger : public osgManipulator::CompositeDragger
		{
			protected:
				osg::ref_ptr<osgManipulator::RotateCylinderDragger> _xDragger;
				osg::ref_ptr<osgManipulator::RotateCylinderDragger> _yDragger;
				osg::ref_ptr<osgManipulator::RotateCylinderDragger> _zDragger;

			virtual ~OsgTrackballDragger(void);

			public:
				OsgTrackballDragger(BOOL bAllowRotateX, BOOL bAllowRotateY, BOOL bAllowRotateZ);

				/** Setup default geometry for dragger. */
				void setupDefaultGeometry();
		};

	}// end Visualization
}// end OsgAnimatSim

