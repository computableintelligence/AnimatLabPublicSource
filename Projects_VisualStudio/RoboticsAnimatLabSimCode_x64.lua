-- A solution contains projects, and defines the available configurations
solution "RoboticsAnimatLabSimCode_x64"
	configurations { "Debug_x64", "Release_x64", "Debug_Double_x64", "Release_Double_x64" }
	platforms {"x64"}
	dofile "..\\Libraries\\BootstrapLoader\\Projects_VisualStudio\\Projects_x64.lua"
	dofile "..\\Libraries\\StdUtils\\Projects_VisualStudio\\Projects_x64.lua"
	dofile "..\\Libraries\\AnimatSim\\Projects_VisualStudio\\Projects_x64.lua"
	dofile "..\\Libraries\\FiringRateSim\\Projects_VisualStudio\\Projects_x64.lua"
	dofile "..\\Libraries\\IntegrateFireSim\\Projects_VisualStudio\\Projects_x64.lua"
	dofile "..\\Libraries\\RoboticsAnimatSim\\Projects_VisualStudio\\Projects_x64.lua"
	dofile "..\\Libraries\\RoboticsAnimatSim\\Projects_VisualStudio\\UnitTests_x64.lua"
	dofile "..\\Applications\\AnimatSimulator\\Projects_VisualStudio\\Projects_x64.lua"

