
	project "RoboticsAnimatSim"
		language "C++"
		kind     "SharedLib"
		files  { "../*.h",
				 "../*.cpp"}
		
		configuration { "Debug or Debug_Double", "windows" }
			includedirs { "../../../include",
						  "../../StdUtils",
						  "../../AnimatSim",
						  "../../../../3rdParty/boost_1_54_0",
						  "../../../../3rdParty/OpenSceneGraph-3.0.1/include",
						  "../../../../3rdParty/DynamixelSDK/x32/import",
						  "../../../../3rdParty/openFrameworksArduino/src"}	  
			libdirs { "../../../lib",
					  "$(OutDir)", 
					  "../../../../3rdParty/boost_1_54_0/lib",
					  "../../../../3rdParty/OpenSceneGraph-3.0.1/lib",
					  "../../../../3rdParty/DynamixelSDK/x32/import" }
			defines { "WIN32", "_DEBUG", "_WINDOWS", "_USRDLL", "ROBOTICSANIMATLIBRARY_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
			flags   { "Symbols", "SEH" }
			targetdir ("Debug")
			targetname ("RoboticsAnimatSim_vc10D")
			links { "wsock32", 
					"netapi32", 
					"comctl32", 
					"wbemuuid",
					"OpenThreadsd", 
					"osgd", 
					"dynamixel",
					"openFrameworksArduinoD" }
			postbuildcommands { "Copy $(OutDir)RoboticsAnimatSim_vc10D.lib ..\\..\\..\\lib\\RoboticsAnimatSim_vc10D.lib", 
			                    "Copy $(TargetPath) ..\\..\\..\\bin" }
	 
		configuration { "Release or Release_Double", "windows" }
			includedirs { "../../../include",
						  "../../StdUtils",
						  "../../AnimatSim",
						  "../../../../3rdParty/boost_1_54_0",
						  "../../../../3rdParty/OpenSceneGraph-3.0.1/include",
						  "../../../../3rdParty/DynamixelSDK/x32/import",
						  "../../../../3rdParty/openFrameworksArduino/src"}	  
			libdirs { "../../../lib",
					  "$(OutDir)", 
					  "../../../../3rdParty/boost_1_54_0/lib",
					  "../../../../3rdParty/OpenSceneGraph-3.0.1/lib",
					  "../../../../3rdParty/DynamixelSDK/x32/import" }
			defines { "WIN32", "NDEBUG", "_WINDOWS", "_USRDLL", "ROBOTICSANIMATLIBRARY_EXPORTS" }
			flags   { "Optimize", "SEH" }
			targetdir ("Release")
			targetname ("RoboticsAnimatSim_vc10")
			links { "wsock32",
					"netapi32",
					"comctl32",
					"wbemuuid",
					"OpenThreads", 
					"osg", 
					"dynamixel",
					"openFrameworksArduino" }
			postbuildcommands { "Copy $(OutDir)RoboticsAnimatSim_vc10.lib ..\\..\\..\\lib\\RoboticsAnimatSim_vc10.lib", 
			                    "Copy $(TargetPath) ..\\..\\..\\bin" }
