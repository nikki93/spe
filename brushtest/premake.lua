---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------

-- Project ----------------------------------------------------------------------------------

project.name = "brushtest"
project.bindir = "bin"

-- Package ----------------------------------------------------------------------------------

package = newpackage()

package.name = "brushtest"
package.kind = "exe"
package.language = "c++"
package.configs = { "Debug", "Release" }

if (windows) then
   table.insert(package.defines, "WIN32") -- Needed to fix something on Windows.
end

-- Include and library search paths, system dependent ---------------------------------------

package.includepaths = {
    -- project
    "./src",

    -- openFrameworks
    "../../Libraries/of/libs/openFrameworks",
    "../../Libraries/of/libs/openFrameworks/video",
    "../../Libraries/of/libs/openFrameworks/types",
    "../../Libraries/of/libs/openFrameworks/communication",
    "../../Libraries/of/libs/openFrameworks/events",
    "../../Libraries/of/libs/openFrameworks/math",
    "../../Libraries/of/libs/openFrameworks/graphics",
    "../../Libraries/of/libs/openFrameworks/3d",
    "../../Libraries/of/libs/openFrameworks/utils",
    "../../Libraries/of/libs/openFrameworks/sound",
    "../../Libraries/of/libs/openFrameworks/gl",
    "../../Libraries/of/libs/openFrameworks/app",

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
    "../../Libraries/of/libs/assimp/include",
    "../../Libraries/of/libs/assimp/include/Compiler",
    "../../Libraries/of/libs/cairo/include",
    "../../Libraries/of/libs/cairo/include/cairo",
    "../../Libraries/of/libs/cairo/include/libpng15",
    "../../Libraries/of/libs/cairo/include/pixman-1",
    "../../Libraries/of/libs/fmodex/include",
    "../../Libraries/of/libs/FreeImage/include",
    "../../Libraries/of/libs/freetype/include",
    "../../Libraries/of/libs/freetype/include/freetype2",
    "../../Libraries/of/libs/freetype/include/freetype2/freetype",
    "../../Libraries/of/libs/freetype/include/freetype2/freetype/config",
    "../../Libraries/of/libs/freetype/include/freetype2/freetype/internal",
    "../../Libraries/of/libs/freetype/include/freetype2/freetype/internal/services",
    "../../Libraries/of/libs/glew/include",
    "../../Libraries/of/libs/glew/include/GL",
    "../../Libraries/of/libs/kiss/include",
    "../../Libraries/of/libs/poco/include",
    "../../Libraries/of/libs/poco/include/Poco",
    "../../Libraries/of/libs/poco/include/Poco/Util",
    "../../Libraries/of/libs/poco/include/Poco/SAX",
    "../../Libraries/of/libs/poco/include/Poco/Dynamic",
    "../../Libraries/of/libs/poco/include/Poco/DOM",
    "../../Libraries/of/libs/poco/include/Poco/Net",
    "../../Libraries/of/libs/poco/include/Poco/XML",
    "../../Libraries/of/libs/poco/include/CppUnit",
    "../../Libraries/of/libs/portaudio/include",
    "../../Libraries/of/libs/rtAudio/include",
    "../../Libraries/of/libs/tess2/include"
}

package.libpaths = {
    "../../Libraries/of/libs/openFrameworksCompiled/lib/linux64",
    "../../Libraries/of/libs/freetype/lib/linux64",
    "../../Libraries/of/libs/kiss/lib/linux64",
    "../../Libraries/of/libs/portaudio/lib/linux64",
    "../../Libraries/of/libs/rtAudio/lib/linux64",
    "../../Libraries/of/libs/tess2/lib/linux64",
    "../../Libraries/of/libs/poco/lib/linux64",
    "../../Libraries/of/libs/poco/lib/linux64",
    "../../Libraries/of/libs/poco/lib/linux64",
    "../../Libraries/of/libs/poco/lib/linux64",
    "../../Libraries/of/libs/fmodex/lib/linux64",
    "../../Libraries/of/libs/freetype/lib/linux64",
    "../../Libraries/of/libs/kiss/lib/linux64",
    "../../Libraries/of/libs/openFrameworksCompiled/lib/linux64",
    "../../Libraries/of/libs/poco/lib/linux64",
    "../../Libraries/of/libs/portaudio/lib/linux64",
    "../../Libraries/of/libs/rtAudio/lib/linux64",
    "../../Libraries/of/libs/tess2/lib/linux64"
}

-- Libraries to link to ---------------------------------------------------------------------

package.links = {
    "openFrameworks",
    "kiss",
    "tess2",
    "PocoNet",
    "PocoXML",
    "PocoUtil",
    "PocoFoundation",
    "mpg123",
    "GLEW",
    "cairo",
    "glut",
    "GL",
    "openal",
    "sndfile",
    "freeimage",
    "GLU"
    --"freetype",
    --"portaudio",
    --"RtAudio",
    --"fmodex",
    --"gtk-x11-2.0",
    --"gdk-x11-2.0",
    --"atk-1.0",
    --"gio-2.0",
    --"pangoft2-1.0",
    --"pangocairo-1.0",
    --"gdk_pixbuf-2.0",
    --"cairo",
    --"pango-1.0",
    --"freetype",
    --"fontconfig",
    --"gobject-2.0",
    --"glib-2.0",
    --"jack",
    --"pthread",
    --"gstvideo-0.10",
    --"gstapp-0.10",
    --"gstbase-0.10",
    --"gstreamer-0.10",
    --"gobject-2.0",
    --"gmodule-2.0",
    --"gthread-2.0",
    --"xml2",
    --"glib-2.0",
    --"udev",
    --"rt",
    --"vorbis",
    --"FLAC",
    --"ogg",
    --"asound"
}

-- pkg-configable stuff ---------------------------------------------------------------------

if (linux) then
package.buildoptions = {
    "-pthread",
    "-DOF_USING_GTK",
    "-DOF_USING_MPG123"
}

package.linkoptions = {
    "-pthread"
}
end

-- Files ------------------------------------------------------------------------------------

package.files = {
matchrecursive("*.h", "*.cpp"),
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
