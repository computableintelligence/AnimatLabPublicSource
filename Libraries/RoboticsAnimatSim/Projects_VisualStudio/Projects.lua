
	project "RoboticsAnimatSim"
		language "C++"
		kind     "SharedLib"
		files  { "../*.h",
				 "../*.cpp"}
		includedirs { "../../../include",
					  "../../StdUtils",
					  "../../AnimatSim",
  					  "../../../../3rdParty/OpenSceneGraph-3.0.1/include"}	  
		libdirs { "../../../lib",
				  "$(OutDir)",
				  "../../../../3rdParty/OpenSceneGraph-3.0.1/lib" }
		
		configuration { "Debug", "windows" }
			defines { "WIN32", "_DEBUG", "_WINDOWS", "_USRDLL", "ROBOTICSANIMATLIBRARY_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
			flags   { "Symbols", "SEH" }
			targetdir ("Debug")
			targetname ("RoboticsAnimatSim_vc10D")
			links { "OpenThreadsd",
					"osgd", 
					"wsock32", 
					"netapi32", 
					"comctl32", 
					"wbemuuid" }
			postbuildcommands { "Copy $(OutDir)RoboticsAnimatSim_vc10D.lib ..\\..\\..\\lib\\RoboticsAnimatSim_vc10D.lib", 
			                    "Copy $(TargetPath) ..\\..\\..\\bin",
								"Copy $(TargetPATH) ..\\..\\..\\unit_test_bin\\$(TargetName)$(TargetExt)" }
	 
		configuration { "Release", "windows" }
			defines { "WIN32", "NDEBUG", "_WINDOWS", "_USRDLL", "ROBOTICSANIMATLIBRARY_EXPORTS" }
			flags   { "Optimize", "SEH" }
			targetdir ("Release")
			targetname ("RoboticsAnimatSim_vc10")
			links { "OpenThreads",
					"osg",
					"wsock32",
					"netapi32",
					"comctl32",
					"wbemuuid" }
			postbuildcommands { "Copy $(OutDir)RoboticsAnimatSim_vc10.lib ..\\..\\..\\lib\\RoboticsAnimatSim_vc10.lib", 
			                    "Copy $(TargetPath) ..\\..\\..\\bin", 
								"Copy $(TargetPATH) ..\\..\\..\\unit_test_bin\\$(TargetName)$(TargetExt)" }

	project "Robotics_UnitTests"
		language "C++"
		kind     "ConsoleApp"
		files  { "../Robotics_UnitTests/*.h",
				 "../Robotics_UnitTests/*.cpp"}
		includedirs { "../../../include",
				      "../../StdUtils",
					  "../../AnimatSim",
					  "../../RoboticsAnimatSim",
  					  "../../../../3rdParty/OpenSceneGraph-3.0.1/include",
					  "../../../../3rdParty/boost_1_54_0"}	  
		libdirs { "../../../lib",
				  "$(OutDir)",
				  "../../../../3rdParty/OpenSceneGraph-3.0.1/lib",
				  "../../../../3rdParty/boost_1_54_0/lib" }
		targetdir ("../../../bin")
		targetname ("Robotics_UnitTests")
		
		configuration { "Debug", "windows" }
			defines { "WIN32", "_DEBUG", "_WINDOWS", "_USRDLL", "_CRT_SECURE_NO_WARNINGS"	}
			flags   { "Symbols", "SEH" }
			links { "OpenThreadsd",
					"osgd", 
					"wsock32", 
					"netapi32", 
					"comctl32", 
					"wbemuuid" }
	 
		configuration { "Release", "windows" }
			defines { "WIN32", "NDEBUG", "_WINDOWS", "_USRDLL" }
			flags   { "Optimize", "SEH" }
			links { "OpenThreads",
					"osg",
					"wsock32",
					"netapi32",
					"comctl32",
					"wbemuuid" }

								
