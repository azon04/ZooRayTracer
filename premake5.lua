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

    files { "Source/*.h", 
            "Source/*.cpp",
            "Source/*.c"
        }

    debugdir "./"
    debugargs { "> output.ppm" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

group "Tools"
    
    