	
	project "StdClassFactoryTester_x64"
		language "C++"
		kind     "SharedLib"
		files  { "../StdClassFactoryTester/*.h",
				 "../StdClassFactoryTester/*.cpp"}
		includedirs { "../../../include", 
					  "../../StdUtils", 
					  "../../../../3rdParty/boost_1_54_0",
					  "../../../../3rdParty/stlsoft-1.9.117/include" }	  
		libdirs { "../../../lib", 
					  "../../../../3rdParty/boost_1_54_0/lib_x64" }
		links { "StdUtils_x64" }
	  
		configuration { "Debug_x64 or Debug_Double_x64", "windows" }
			defines { "WIN32", "_DEBUG", "_WINDOWS", "_USRDLL", "STDCLASSFACTORYTESTER_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
			flags   { "Symbols", "SEH" }
			targetdir ("Debug_x64")
			targetname ("StdClassFactoryTester_x64")
			postbuildcommands { "Copy $(TargetPATH) ..\\..\\..\\bin_x64\\$(TargetName)$(TargetExt)" }
	 
		configuration { "Release_x64 or Release_Double_x64", "windows" }
			defines { "WIN32", "NDEBUG", "_WINDOWS", "_USRDLL", "STDCLASSFACTORYTESTER_EXPORTS" }
			flags   { "Optimize", "SEH" }
			targetdir ("Release_x64")
			targetname ("StdClassFactoryTester_x64")
			postbuildcommands { "Copy $(TargetPATH) ..\\..\\..\\bin_x64\\$(TargetName)$(TargetExt)" }
	