
	project "FiringRateSim"
		language "C++"
		kind     "SharedLib"
		files  { "../*.h",
				 "../*.cpp"}
		buildoptions { "-std=c++0x" }
		includedirs { "../../../include", 
					  "../../StdUtils", 
					  "../../AnimatSim",
					  "../../../../3rdParty/stlsoft-1.9.117/include" }
		libdirs { "../../../bin" }
		links { "dl"}
	  
		configuration { "Debug or Debug_Double", "linux" }
			defines { "_DEBUG", "FASTNEURALNET_EXPORTS"	}
			flags   { "Symbols", "SEH" }
			targetdir ("Debug")
			targetname ("FiringRateSim_debug")
			links { "StdUtils_debug", 
					"AnimatSim_debug"}
			postbuildcommands { "cp Debug/libFiringRateSim_debug.so ../../../bin" }
	 
		configuration { "Release or Release_Double", "linux" }
			defines { "NDEBUG", "FASTNEURALNET_EXPORTS" }
			flags   { "Optimize", "SEH" }
			targetdir ("Release")
			targetname ("FiringRateSim")
			links { "StdUtils",
					"AnimatSim"}
			postbuildcommands { "cp Release/libFiringRateSim.so ../../../bin" }
