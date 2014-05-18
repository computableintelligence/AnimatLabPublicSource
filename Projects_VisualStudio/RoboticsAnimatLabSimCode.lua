-- A solution contains projects, and defines the available configurations
solution "RoboticsAnimatSimCode"
	configurations { "Debug", "Release", "Debug_Double", "Release_Double", "Debug_Static" }
	dofile "..\\Libraries\\BootstrapLoader\\Projects_VisualStudio\\Projects.lua"
	dofile "..\\Libraries\\StdUtils\\Projects_VisualStudio\\Projects.lua"
	dofile "..\\Libraries\\AnimatSim\\Projects_VisualStudio\\Projects.lua"
	dofile "..\\Libraries\\FiringRateSim\\Projects_VisualStudio\\Projects.lua"
	dofile "..\\Libraries\\IntegrateFireSim\\Projects_VisualStudio\\Projects.lua"
	dofile "..\\Libraries\\RoboticsAnimatSim\\Projects_VisualStudio\\Projects.lua"
	dofile "..\\Applications\\AnimatSimulator\\Projects_VisualStudio\\Projects.lua"