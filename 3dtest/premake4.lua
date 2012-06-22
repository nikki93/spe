-- Project ----------------------------------------------------------------------------------

solution "3dtest"
configurations { "Debug", "Release" }

project "3dtest"
targetname "3dtest"
kind "ConsoleApp"
language "C++"

-- Files ------------------------------------------------------------------------------------

files { "**.h", "**.cpp", "**.c" }

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

configuration "Debug"
    flags { "Symbols" }
    defines { "DEBUG", "_DEBUG" }
    objdir "obj/debug"
    targetdir "bin/debug"
    targetsuffix "_d"

configuration "Release"
    objdir "obj/release"
    targetdir "bin/release"

---------------------------------------------------------------------------------------------

end

---------------------------------------------------------------------------------------------
--- Windows ---------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------

if os.is('windows') then

defines { "WIN32", "_CONSOLE", "POCO_STATIC" }

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

    ofpath .. "/libs/glut/include",
    ofpath .. "/libs/rtAudio/include",
    ofpath .. "/libs/quicktime/include",
    ofpath .. "/libs/freetype/include",
    ofpath .. "/libs/freetype/include/freetype2",
    ofpath .. "/libs/FreeImage/include",
    ofpath .. "/libs/fmodex/include",
    ofpath .. "/libs/videoInput/include",
    ofpath .. "/libs/glu/include",
    ofpath .. "/libs/glew/include",
    ofpath .. "/libs/tess2/include",
    ofpath .. "/libs/cairo/include/cairo",
    ofpath .. "/libs/Poco/include",
    ofpath .. "/addons",
}

libdirs  {
    ofpath .. "/libs/openFrameworksCompiled/lib/vs2010",

    ofpath .. "/libs/glut/lib/vs2010",
    ofpath .. "/libs/rtAudio/lib/vs2010",
    ofpath .. "/libs/FreeImage/lib/vs2010",
    ofpath .. "/libs/freetype/lib/vs2010",
    ofpath .. "/libs/quicktime/lib/vs2010",
    ofpath .. "/libs/fmodex/lib/vs2010",
    ofpath .. "/libs/videoInput/lib/vs2010",
    ofpath .. "/libs/cairo/lib/vs2010",
    ofpath .. "/libs/glu/lib/vs2010",
    ofpath .. "/libs/glew/lib/vs2010",
    ofpath .. "/libs/Poco/lib/vs2010",
    ofpath .. "/libs/tess2/lib/vs2010"
}

links  { 
    "cairo-static", "pixman-1", "msimg32", "OpenGL32", "GLu32", "setupapi", "Vfw32", 
    "comctl32", "glut32", "rtAudioD", "videoInput", "libfreetype", "FreeImage",
    "qtmlClient", "dsound", "glew32s", "fmodex_vc", "glu32", "PocoFoundationmdd", 
    "PocoNetmdd", "PocoUtilmdd", "PocoXMLmdd", "Ws2_32", "tess2"
}

-- Build options ----------------------------------------------------------------------------

buildoptions "/MP"
linkoptions { "/NODEFAULTLIB:libcpmt.lib", "/NODEFAULTLIB:LIBCMT", "/SUBSYSTEM:WINDOWS" }

-- Configurations ---------------------------------------------------------------------------

configuration "Debug"
    flags { "Symbols" }
    defines { "DEBUG", "_DEBUG" }
    links "openframeworksLibDebug"
    objdir "obj/debug"
    targetdir "bin/debug"
    targetsuffix "_d"

configuration "Release"
    links "openframeworksLib"
    objdir "obj/release"
    targetdir "bin/release"

---------------------------------------------------------------------------------------------

end

