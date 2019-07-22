-- premake5.lua

-- Actions
newaction {
    trigger = "clean",
    description = "clean up project files",
    execute = function()
        os.rmdir("./Projects")
    end
}

workspace "ZooRayTracer"
    location ("Projects/" .. _ACTION)
    configurations { "Debug", "Release" }

project "ZooRayTracer"
    location ("Projects/".. _ACTION .."/ZooRayTracer")
    kind "ConsoleApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"

    includedirs { "Source",
        "Source/Framework",
        "Source/Math",
        "Source/Lib/Include",
        "3rdParty/Build/assimp/include"
    }

    files { "Source/**.h", 
            "Source/**.cpp",
            "Source/**.c"
        }

    debugdir "./"
    debugargs { "> Output/output.ppm" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter { "Debug", "action:vs2017" }
        syslibdirs { "3rdParty/Build/assimp/Debug/x86-vs141" }
        links { "assimp-vc141-mtd" }
        postbuildcommands {
            "{COPY} " .. path.getabsolute("3rdParty/Build/assimp/Debug/x86-vs141/assimp-vc141-mtd.dll") .." %{cfg.targetdir}"
        }

    filter { "Debug", "action:vs2015" }
        syslibdirs { "3rdParty/Build/assimp/Debug/x86-vs140" }
        links { "assimp-vc140-mtd" }
        postbuildcommands {
            "{COPY} " .. path.getabsolute("3rdParty/Build/assimp/Debug/x86-vs140/assimp-vc140-mtd.dll") .." %{cfg.targetdir}"
        }

group "Tools"
    
    