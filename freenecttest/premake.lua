---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------

-- Project ----------------------------------------------------------------------------------

project.name = "freenecttest"
project.bindir = "bin"

-- Package ----------------------------------------------------------------------------------

package = newpackage()

package.name = "freenecttest"
package.kind = "exe"
package.language = "c++"
package.configs = { "Debug", "Release" }

-- Files ------------------------------------------------------------------------------------

package.files = {
matchrecursive("*.h", "*.cpp"),
}

---------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------
--- Linux -----------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------

if (linux) then 

-- Include and library search paths ---------------------------------------------------------

ofpath = "../../../Libraries/of"

package.includepaths = {
    -- project
    "./src",

    -- openFrameworks
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

    -- openFrameworks dependencies
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
    ofpath .. "/libs/tess2/include",

    -- freenect
    "/usr/include/libfreenect"
}

package.libpaths = {
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

-- Libraries --------------------------------------------------------------------------------

package.links = {
    "openFrameworks", "kiss", "tess2", "PocoNet", "PocoXML", "PocoUtil", "PocoFoundation",
    "mpg123", "GLEW", "cairo", "glut", "GL", "openal", "sndfile", "freeimage", "GLU",

    "freenect"
}

-- Build options ----------------------------------------------------------------------------

package.buildoptions = { "-pthread", "-DOF_USING_GTK", "-DOF_USING_MPG123" }
package.linkoptions = { "-pthread" }

-- Debug configuration ----------------------------------------------------------------------

debug = package.config["Debug"]
debug.defines = { "DEBUG", "_DEBUG" }
debug.objdir = "obj/debug"
debug.target = "debug/" .. package.name .. "_d"

debug.buildoptions = { "-g" }

-- Release configuration --------------------------------------------------------------------

release = package.config["Release"]
release.objdir = "obj/release"
release.target = "release/" .. package.name

---------------------------------------------------------------------------------------------

end

---------------------------------------------------------------------------------------------
--- Windows ---------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------

if (windows) then

table.insert(package.defines, "WIN32")      -- To fix a problem on Windows

-- Include and library search paths ---------------------------------------------------------

ofpath = "../../Libraries/of"

package.includepaths = {
    -- project
    "./src",

    -- openFrameworks
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

    -- openFrameworks dependencies
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

package.libpaths = {
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

-- Libraries --------------------------------------------------------------------------------

package.links = {
    "openFrameworks", "kiss", "tess2", "PocoNet", "PocoXML", "PocoUtil", "PocoFoundation",
    "mpg123", "GLEW", "cairo", "glut", "GL", "openal", "sndfile", "freeimage", "GLU"
}

-- Build options ----------------------------------------------------------------------------

package.buildoptions = { "/MP" }
package.linkoptions = { 
    "/NODEFAULTLIB:libcpmt.lib", "/NODEFAULTLIB:LIBCMT", "/SUBSYSTEM:WINDOWS" 
}

-- Debug configuration ----------------------------------------------------------------------

debug = package.config["Debug"]
debug.defines = { "DEBUG", "_DEBUG" }
debug.objdir = "obj/debug"
debug.target = "debug/" .. package.name .. "_d"

debug.buildoptions = { "-g" }

-- Release configuration --------------------------------------------------------------------

release = package.config["Release"]
release.objdir = "obj/release"
release.target = "release/" .. package.name

---------------------------------------------------------------------------------------------

end

