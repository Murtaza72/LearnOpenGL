workspace "LearnOpenGL"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

	startproject "LearnOpenGL"

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

include "LearnOpenGL"