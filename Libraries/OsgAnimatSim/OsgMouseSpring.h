#pragma once

namespace OsgAnimatSim
{
	namespace Visualization
	{

class OsgMouseSpring
{
	protected:
		osg::ref_ptr<osg::Geode> m_gdeLine;
		osg::ref_ptr<osg::Geometry> m_linesGeom;
		osg::ref_ptr<osg::LineWidth> m_lineWidth;
		osg::Vec3 m_v3Start;
		osg::Vec3 m_v3End;
		osg::ref_ptr<osg::Vec3Array> m_aryLines;
		osg::Vec3 m_v3Grab;
        RigidBody *m_lpRB;
		OsgMovableItem *m_osgRB;

		void Update();

	public:
		OsgMouseSpring(void);
		~OsgMouseSpring(void);

		osg::Node* GetNode(){return m_gdeLine.get();};
		void SetStart(osg::Vec3 v3Start);
		osg::Vec3 GetStart() { return m_v3Start; };
		
		void SetEnd(osg::Vec3 v3End);
		osg::Vec3 GetEnd() { return m_v3End; };

		void SetGrabPosition(osg::Vec3 v3Grab) {m_v3Grab = v3Grab;};
		osg::Vec3 GetGrabPosition() {return m_v3Grab;}

		void SetRigidBody (RigidBody *lpRB) 
        {
            m_lpRB = lpRB;
            m_osgRB = dynamic_cast<OsgMovableItem *>(lpRB);
        };
		RigidBody* GetRigidBody() {return m_lpRB;};
		OsgMovableItem* GetMovableItem() {return m_osgRB;};

		void Visible(bool bVal);
		void Initialize();
};

	}// end Visualization
}// end OsgAnimatSim
