-- Project ----------------------------------------------------------------------------------

solution "brushtest"
configurations { "debug", "release" }

project "brushtest"
targetname "brushtest"
kind "ConsoleApp"
language "C++"

defines { "OF_USING_GTK", "OF_USING_MPG123" }

-- Files ------------------------------------------------------------------------------------

files { "**.h", "**.cpp" }

---------------------------------------------------------------------------------------------
--- Linux -----------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------

if os.is('linux') then

-- Include/library directories and libraries ------------------------------------------------

ofpath = "../../../Libraries/of"

includedirs { "./src", ofpath .. "/libs/**" }
libdirs  { ofpath .. "/libs/**" }

links  { "openFrameworks", "kiss", "tess2", "PocoNet", "PocoXML", "PocoUtil", 
    "PocoFoundation", "mpg123", "GLEW", "cairo", "glut", "GL", "openal", "sndfile", 
    "freeimage", "GLU" }

-- Build options ----------------------------------------------------------------------------

buildoptions "-pthread"
linkoptions "-pthread"

-- Configurations ---------------------------------------------------------------------------

configuration "debug"
    buildoptions { "-g" }
    defines { "DEBUG", "_DEBUG" }
    objdir "obj/debug"
    targetdir "bin/debug"
    targetsuffix "_d"

configuration "release"
    objdir "obj/release"
    targetdir "bin/release"

---------------------------------------------------------------------------------------------

end

---------------------------------------------------------------------------------------------
--- Windows ---------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------

if os.is('windows') then

defines "WIN32"

-- Include/library directories and libraries ------------------------------------------------

ofpath = "../../of"

includedirs { "./src", ofpath .. "/libs/**" }
libdirs  { ofpath .. "/libs/**" }

links  { "openFrameworks", "kiss", "tess2", "PocoNet", "PocoXML", "PocoUtil", 
    "PocoFoundation", "mpg123", "GLEW", "cairo", "glut", "GL", "openal", "sndfile", 
    "freeimage", "GLU", "pthread" }

-- Build options ----------------------------------------------------------------------------

buildoptions "/MP"
linkoptions { "/NODEFAULTLIB:libcpmt.lib", "/NODEFAULTLIB:LIBCMT", "/SUBSYSTEM:WINDOWS" }

-- Configurations ---------------------------------------------------------------------------

configuration "debug"
    buildoptions { "-g" }
    defines { "DEBUG", "_DEBUG" }
    objdir "obj/debug"
    targetdir "bin/debug"
    targetsuffix "_d"

configuration "release"
    objdir "obj/release"
    targetdir "bin/release"

---------------------------------------------------------------------------------------------

end

