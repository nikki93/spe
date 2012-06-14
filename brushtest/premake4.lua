-- Project ----------------------------------------------------------------------------------

solution "brushtest"
configurations { "debug", "release" }

project "brushtest"
targetname "brushtest"
kind "ConsoleApp"
language "C++"

-- Files ------------------------------------------------------------------------------------

files { "**.h", "**.cpp" }

---------------------------------------------------------------------------------------------
--- Linux -----------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------

if os.is('linux') then

defines { "OF_USING_GTK", "OF_USING_MPG123" }

-- Include/library directories and libraries ------------------------------------------------

ofpath = "../../../Libraries/of"

includedirs { 
    -- project
    "./src",

    ofpath .. "/libs/openFrameworks",
    ofpath .. "/libs/openFrameworks/video",
    ofpath .. "/libs/openFrameworks/types",
    ofpath .. "/libs/openFrameworks/communication",
    ofpath .. "/libs/openFrameworks/events",
    ofpath .. "/libs/openFrameworks/math",
    ofpath .. "/libs/openFrameworks/graphics",
    ofpath .. "/libs/openFrameworks/3d",
    ofpath .. "/libs/openFrameworks/utils",
    ofpath .. "/libs/openFrameworks/sound",
    ofpath .. "/libs/openFrameworks/gl",
    ofpath .. "/libs/openFrameworks/app",
    "/usr/include/gtk-2.0",
    "/usr/lib/gtk-2.0/include",
    "/usr/include/atk-1.0",
    "/usr/include/cairo",
    "/usr/include/gdk-pixbuf-2.0",
    "/usr/include/pango-1.0",
    "/usr/include/glib-2.0",
    "/usr/lib/glib-2.0/include",
    "/usr/include/pixman-1",
    "/usr/include/freetype2",
    "/usr/include/libpng15",
    "/usr/include/GL",
    "/usr/include/gstreamer-0.10",
    "/usr/include/glib-2.0",
    "/usr/lib/glib-2.0/include",
    "/usr/include/libxml2",
    ofpath .. "/libs/assimp/include",
    ofpath .. "/libs/assimp/include/Compiler",
    ofpath .. "/libs/cairo/include",
    ofpath .. "/libs/cairo/include/cairo",
    ofpath .. "/libs/cairo/include/libpng15",
    ofpath .. "/libs/cairo/include/pixman-1",
    ofpath .. "/libs/fmodex/include",
    ofpath .. "/libs/FreeImage/include",
    ofpath .. "/libs/freetype/include",
    ofpath .. "/libs/freetype/include/freetype2",
    ofpath .. "/libs/freetype/include/freetype2/freetype",
    ofpath .. "/libs/freetype/include/freetype2/freetype/config",
    ofpath .. "/libs/freetype/include/freetype2/freetype/internal",
    ofpath .. "/libs/freetype/include/freetype2/freetype/internal/services",
    ofpath .. "/libs/glew/include",
    ofpath .. "/libs/glew/include/GL",
    ofpath .. "/libs/kiss/include",
    ofpath .. "/libs/poco/include",
    ofpath .. "/libs/poco/include/Poco",
    ofpath .. "/libs/poco/include/Poco/Util",
    ofpath .. "/libs/poco/include/Poco/SAX",
    ofpath .. "/libs/poco/include/Poco/Dynamic",
    ofpath .. "/libs/poco/include/Poco/DOM",
    ofpath .. "/libs/poco/include/Poco/Net",
    ofpath .. "/libs/poco/include/Poco/XML",
    ofpath .. "/libs/poco/include/CppUnit",
    ofpath .. "/libs/portaudio/include",
    ofpath .. "/libs/rtAudio/include",
    ofpath .. "/libs/tess2/include"
}

libdirs  {
    ofpath .. "/libs/openFrameworksCompiled/lib/linux64",
    ofpath .. "/libs/freetype/lib/linux64",
    ofpath .. "/libs/kiss/lib/linux64",
    ofpath .. "/libs/portaudio/lib/linux64",
    ofpath .. "/libs/rtAudio/lib/linux64",
    ofpath .. "/libs/tess2/lib/linux64",
    ofpath .. "/libs/poco/lib/linux64",
    ofpath .. "/libs/poco/lib/linux64",
    ofpath .. "/libs/poco/lib/linux64",
    ofpath .. "/libs/poco/lib/linux64",
    ofpath .. "/libs/fmodex/lib/linux64",
    ofpath .. "/libs/freetype/lib/linux64",
    ofpath .. "/libs/kiss/lib/linux64",
    ofpath .. "/libs/openFrameworksCompiled/lib/linux64",
    ofpath .. "/libs/poco/lib/linux64",
    ofpath .. "/libs/portaudio/lib/linux64",
    ofpath .. "/libs/rtAudio/lib/linux64",
    ofpath .. "/libs/tess2/lib/linux64"
}

links  { 
    "openFrameworks", "kiss", "tess2", "PocoNet", "PocoXML", "PocoUtil", 
    "PocoFoundation", "mpg123", "GLEW", "cairo", "glut", "GL", "openal", "sndfile", 
    "freeimage", "GLU" 
}

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

includedirs { 
    -- project
    "./src",

    ofpath .. "/libs/openFrameworks",
    ofpath .. "/libs/openFrameworks/video",
    ofpath .. "/libs/openFrameworks/types",
    ofpath .. "/libs/openFrameworks/communication",
    ofpath .. "/libs/openFrameworks/events",
    ofpath .. "/libs/openFrameworks/math",
    ofpath .. "/libs/openFrameworks/graphics",
    ofpath .. "/libs/openFrameworks/3d",
    ofpath .. "/libs/openFrameworks/utils",
    ofpath .. "/libs/openFrameworks/sound",
    ofpath .. "/libs/openFrameworks/gl",
    ofpath .. "/libs/openFrameworks/app",
    ofpath .. "/libs/assimp/include",
    ofpath .. "/libs/assimp/include/Compiler",
    ofpath .. "/libs/cairo/include",
    ofpath .. "/libs/cairo/include/cairo",
    ofpath .. "/libs/cairo/include/libpng15",
    ofpath .. "/libs/cairo/include/pixman-1",
    ofpath .. "/libs/fmodex/include",
    ofpath .. "/libs/FreeImage/include",
    ofpath .. "/libs/freetype/include",
    ofpath .. "/libs/freetype/include/freetype2",
    ofpath .. "/libs/freetype/include/freetype2/freetype",
    ofpath .. "/libs/freetype/include/freetype2/freetype/config",
    ofpath .. "/libs/freetype/include/freetype2/freetype/internal",
    ofpath .. "/libs/freetype/include/freetype2/freetype/internal/services",
    ofpath .. "/libs/glew/include",
    ofpath .. "/libs/glew/include/GL",
    ofpath .. "/libs/kiss/include",
    ofpath .. "/libs/poco/include",
    ofpath .. "/libs/poco/include/Poco",
    ofpath .. "/libs/poco/include/Poco/Util",
    ofpath .. "/libs/poco/include/Poco/SAX",
    ofpath .. "/libs/poco/include/Poco/Dynamic",
    ofpath .. "/libs/poco/include/Poco/DOM",
    ofpath .. "/libs/poco/include/Poco/Net",
    ofpath .. "/libs/poco/include/Poco/XML",
    ofpath .. "/libs/poco/include/CppUnit",
    ofpath .. "/libs/portaudio/include",
    ofpath .. "/libs/rtAudio/include",
    ofpath .. "/libs/tess2/include"
}

libdirs  {
    ofpath .. "/libs/openFrameworksCompiled/lib/linux64",
    ofpath .. "/libs/freetype/lib/linux64",
    ofpath .. "/libs/kiss/lib/linux64",
    ofpath .. "/libs/portaudio/lib/linux64",
    ofpath .. "/libs/rtAudio/lib/linux64",
    ofpath .. "/libs/tess2/lib/linux64",
    ofpath .. "/libs/poco/lib/linux64",
    ofpath .. "/libs/poco/lib/linux64",
    ofpath .. "/libs/poco/lib/linux64",
    ofpath .. "/libs/poco/lib/linux64",
    ofpath .. "/libs/fmodex/lib/linux64",
    ofpath .. "/libs/freetype/lib/linux64",
    ofpath .. "/libs/kiss/lib/linux64",
    ofpath .. "/libs/openFrameworksCompiled/lib/linux64",
    ofpath .. "/libs/poco/lib/linux64",
    ofpath .. "/libs/portaudio/lib/linux64",
    ofpath .. "/libs/rtAudio/lib/linux64",
    ofpath .. "/libs/tess2/lib/linux64"
}

links  { 
    "openFrameworks", "kiss", "tess2", "PocoNet", "PocoXML", "PocoUtil", 
    "PocoFoundation", "mpg123", "GLEW", "cairo", "glut", "GL", "openal", "sndfile", 
    "freeimage", "GLU", "pthread"
}

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

