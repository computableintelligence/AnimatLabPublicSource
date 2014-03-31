
	project "BulletAnimatSim_x64"
		language "C++"
		kind     "SharedLib"
		files  { "../*.h",
				 "../*.cpp"}							

		configuration { "Debug_x64", "windows" }
			includedirs { "../../../include",
						  "../../../../3rdParty/OpenSceneGraph-3.0.1_x64/include",
						  "../../../../3rdParty/osgWorks_03_00_00/include",
						  "../../../../3rdParty/Bullet-2.82/src",
						  "../../../../3rdParty/osgBullet_03_00_00/include",
						  "../../StdUtils",
						  "../../AnimatSim",
						  "../../OsgAnimatSim",
						  "../../../../3rdParty/boost_1_54_0"}	  
			libdirs { "../../../lib",
					  "$(OutDir)",
					  "../../../../3rdParty/OpenSceneGraph-3.0.1_x64/lib",
					  "../../../../3rdParty/osgWorks_03_00_00/lib",
					  "../../../../3rdParty/Bullet-2.82/lib",
					  "../../../../3rdParty/osgBullet_03_00_00/lib", 
					  "../../../../3rdParty/boost_1_54_0/lib_x64" }
			defines { "WIN32", "_DEBUG", "_WINDOWS", "_USRDLL", "OSGBULLET_STATIC", "BULLETANIMATLIBRARY_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
			flags   { "Symbols", "SEH" }
			targetdir ("Debug_x64")
			targetname ("BulletAnimatSim_vc10D_single_x64")
			links { "OpenThreadsd", 
					"osgAnimationd", 
					"osgd", 
					"osgDBd", 
					"osgFXd", 
					"osgGAd", 
					"osgManipulatord", 
					"osgParticled", 
					"osgShadowd", 
					"osgSimd", 
					"osgTerraind", 
					"osgTextd", 
					"osgUtild", 
					"osgViewerd", 
					"osgVolumed", 
					"osgWidgetd", 
					"opengl32",
					"osgbDynamicsd_single_x64",
					"osgbCollisiond_single_x64",
					"osgwControlsd_x64",
					"osgwQueryd_x64",
					"osgwToolsd_x64",
					"BulletDynamics_vs2010_single_x64_debug",
					"BulletCollision_vs2010_single_x64_debug",
					"LinearMath_vs2010_single_x64_debug",
					"BulletSoftBody_vs2010_single_x64_debug",
					"wsock32", 
					"netapi32", 
					"comctl32", 
					"wbemuuid" }
			postbuildcommands { "Copy $(OutDir)BulletAnimatSim_vc10D_single_x64.lib ..\\..\\..\\lib\\BulletAnimatSim_vc10D_single_x64.lib", 
			                    "Copy $(TargetPath) ..\\..\\..\\bin_x64" }								
								

		configuration { "Debug_Double_x64", "windows" }
			includedirs { "../../../include",
						  "../../../../3rdParty/OpenSceneGraph-3.0.1_x64/include",
						  "../../../../3rdParty/osgWorks_03_00_00/include",
						  "../../../../3rdParty/Bullet-2.82/src",
						  "../../../../3rdParty/osgBullet_03_00_00/include",
						  "../../StdUtils",
						  "../../AnimatSim",
						  "../../OsgAnimatSim",
						  "../../../../3rdParty/boost_1_54_0"}	  
			libdirs { "../../../lib",
					  "$(OutDir)",
					  "../../../../3rdParty/OpenSceneGraph-3.0.1_x64/lib",
					  "../../../../3rdParty/osgWorks_03_00_00/lib",
					  "../../../../3rdParty/Bullet-2.82/lib",
					  "../../../../3rdParty/osgBullet_03_00_00/lib", 
					  "../../../../3rdParty/boost_1_54_0/lib_x64" }
			defines { "WIN32", "_DEBUG", "_WINDOWS", "_USRDLL", "OSGBULLET_STATIC", "BULLETANIMATLIBRARY_EXPORTS", "BT_USE_DOUBLE_PRECISION", "_CRT_SECURE_NO_WARNINGS" }
			flags   { "Symbols", "SEH" }
			targetdir ("Debug_x64")
			targetname ("BulletAnimatSim_vc10D_double_x64")
			links { "OpenThreadsd", 
					"osgAnimationd", 
					"osgd", 
					"osgDBd", 
					"osgFXd", 
					"osgGAd", 
					"osgManipulatord", 
					"osgParticled", 
					"osgShadowd", 
					"osgSimd", 
					"osgTerraind", 
					"osgTextd", 
					"osgUtild", 
					"osgViewerd", 
					"osgVolumed", 
					"osgWidgetd", 
					"opengl32",
					"osgbDynamicsd_double_x64",
					"osgbCollisiond_double_x64",
					"osgwControlsd",
					"osgwQueryd",
					"osgwToolsd",
					"BulletDynamics_vs2010_double_x64_debug",
					"BulletCollision_vs2010_double_x64_debug",
					"LinearMath_vs2010_double_x64_debug",
					"BulletSoftBody_vs2010_double_x64_debug",
					"wsock32", 
					"netapi32", 
					"comctl32", 
					"wbemuuid" }
			postbuildcommands { "Copy $(OutDir)BulletAnimatSim_vc10D_double_x64.lib ..\\..\\..\\lib\\BulletAnimatSim_vc10D_double_x64.lib", 
			                    "Copy $(TargetPath) ..\\..\\..\\bin_x64" }								


		configuration { "Release_x64", "windows" }
			includedirs { "../../../include",
						  "../../../../3rdParty/OpenSceneGraph-3.0.1_x64/include",
						  "../../../../3rdParty/osgWorks_03_00_00/include",
						  "../../../../3rdParty/Bullet-2.82/src",
						  "../../../../3rdParty/osgBullet_03_00_00/include",
						  "../../StdUtils",
						  "../../AnimatSim",
						  "../../OsgAnimatSim",
						  "../../../../3rdParty/boost_1_54_0"}	  
			libdirs { "../../../lib",
					  "$(OutDir)",
					  "../../../../3rdParty/OpenSceneGraph-3.0.1_x64/lib",
					  "../../../../3rdParty/osgWorks_03_00_00/lib",
					  "../../../../3rdParty/Bullet-2.82/lib",
					  "../../../../3rdParty/osgBullet_03_00_00/lib", 
					  "../../../../3rdParty/boost_1_54_0/lib_x64" }
			defines { "WIN32", "NDEBUG", "_WINDOWS", "_USRDLL", "OSGBULLET_STATIC", "BULLETANIMATLIBRARY_EXPORTS" }
			flags   { "Optimize", "SEH" }
			targetdir ("Release_x64")
			targetname ("BulletAnimatSim_vc10_single_x64")
			links { "OpenThreads",
					"osgAnimation",
					"osg",
					"osgDB",
					"osgFX",
					"osgGA",
					"osgManipulator",
					"osgParticle",
					"osgShadow",
					"osgSim",
					"osgTerrain",
					"osgText",
					"osgUtil",
					"osgViewer",
					"osgVolume",
					"osgWidget",
					"opengl32",
					"osgbDynamics_single_x64",
					"osgbCollision_single_x64",
					"osgwControls",
					"osgwQuery",
					"osgwTools",
					"BulletDynamics_vs2010_single_x64_Release",
					"BulletCollision_vs2010_single_x64_Release",
					"LinearMath_vs2010_single_x64_Release",
					"BulletSoftBody_vs2010_single_x64_Release",
					"wsock32",
					"netapi32",
					"comctl32",
					"wbemuuid" }
			postbuildcommands { "Copy $(OutDir)BulletAnimatSim_vc10_single_x64.lib ..\\..\\..\\lib\\BulletAnimatSim_vc10_single_x64.lib", 
			                    "Copy $(TargetPath) ..\\..\\..\\bin_x64" }
								
		configuration { "Release_Double_x64", "windows" }
			includedirs { "../../../include",
						  "../../../../3rdParty/OpenSceneGraph-3.0.1_x64/include",
						  "../../../../3rdParty/osgWorks_03_00_00/include",
						  "../../../../3rdParty/Bullet-2.82/src",
						  "../../../../3rdParty/osgBullet_03_00_00/include",
						  "../../StdUtils",
						  "../../AnimatSim",
						  "../../OsgAnimatSim",
						  "../../../../3rdParty/boost_1_54_0"}	  
			libdirs { "../../../lib",
					  "$(OutDir)",
					  "../../../../3rdParty/OpenSceneGraph-3.0.1_x64/lib",
					  "../../../../3rdParty/osgWorks_03_00_00/lib",
					  "../../../../3rdParty/Bullet-2.82/lib",
					  "../../../../3rdParty/osgBullet_03_00_00/lib", 
					  "../../../../3rdParty/boost_1_54_0/lib_x64" }
			defines { "WIN32", "NDEBUG", "_WINDOWS", "_USRDLL", "OSGBULLET_STATIC", "BULLETANIMATLIBRARY_EXPORTS", "BT_USE_DOUBLE_PRECISION" }
			flags   { "Optimize", "SEH" }
			targetdir ("Release_x64")
			targetname ("BulletAnimatSim_vc10_double_x64")
			links { "OpenThreads",
					"osgAnimation",
					"osg",
					"osgDB",
					"osgFX",
					"osgGA",
					"osgManipulator",
					"osgParticle",
					"osgShadow",
					"osgSim",
					"osgTerrain",
					"osgText",
					"osgUtil",
					"osgViewer",
					"osgVolume",
					"osgWidget",
					"opengl32",
					"osgbDynamics_double_x64",
					"osgbCollision_double_x64",
					"osgwControls",
					"osgwQuery",
					"osgwTools",
					"BulletDynamics_vs2010_double_x64_Release",
					"BulletCollision_vs2010_double_x64_Release",
					"LinearMath_vs2010_double_x64_Release",
					"BulletSoftBody_vs2010_double_x64_Release",
					"wsock32",
					"netapi32",
					"comctl32",
					"wbemuuid" }
			postbuildcommands { "Copy $(OutDir)BulletAnimatSim_vc10_double_x64.lib ..\\..\\..\\lib\\BulletAnimatSim_vc10_double_x64.lib", 
			                    "Copy $(TargetPath) ..\\..\\..\\bin_x64" }
