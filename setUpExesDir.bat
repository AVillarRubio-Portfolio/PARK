rem Ramón Arjona Quiñones, 2019
rem Script de compilación para ProyectoIII que crea la solución de VS con CMake, compila Ogre y mueve los dll necesarios para la ejecución del mismo
@echo on

mkdir tmp
Xcopy exes\*.cfg tmp
del /q exes\*

rem Librerías dinámicas para OGRE
Xcopy Dependencias\ogre-1.11.5\Build\bin\debug\OgreMain_d.dll exes
Xcopy Dependencias\ogre-1.11.5\Build\bin\release\OgreMain.dll exes
Xcopy Dependencias\ogre-1.11.5\Build\bin\debug\RenderSystem_GL_d.dll exes
Xcopy Dependencias\ogre-1.11.5\Build\bin\release\RenderSystem_GL.dll exes
Xcopy Dependencias\ogre-1.11.5\Build\bin\release\zlib.dll exes
Xcopy Dependencias\ogre-1.11.5\Build\bin\debug\Codec_STBI_d.dll exes
Xcopy Dependencias\ogre-1.11.5\Build\bin\release\Codec_STBI.dll exes
Xcopy Dependencias\ogre-1.11.5\Build\bin\debug\OgreTerrain_d.dll exes
Xcopy Dependencias\ogre-1.11.5\Build\bin\release\OgreTerrain.dll exes
Xcopy Dependencias\ogre-1.11.5\Build\bin\debug\OgrePaging_d.dll exes
Xcopy Dependencias\ogre-1.11.5\Build\bin\release\OgrePaging.dll exes
Xcopy Dependencias\ogre-1.11.5\Build\SDL2-build\RelWithDebInfo\SDL2.dll exes

rem Librerías dinámicas para CEGUI
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUIBase-0.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUIBase-0_d.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUICommonDialogs-0.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUICommonDialogs-0_d.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUICoreWindowRendererSet.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUICoreWindowRendererSet_d.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUIOgreRenderer-0.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUIOgreRenderer-0_d.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUISILLYImageCodec.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUISILLYImageCodec_d.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUIOpenGLRenderer-0.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUIOpenGLRenderer-0_d.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUIExpatParser.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUIExpatParser_d.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUIDirect3D9Renderer-0.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUIDirect3D9Renderer-0_d.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUIDirect3D10Renderer-0.dll exes
Xcopy Dependencias\cegui-0.8.7\Build\bin\CEGUIDirect3D10Renderer-0_d.dll exes
Xcopy Dependencias\cegui-0.8.7\dependencies\bin\freetype_d.dll exes
Xcopy Dependencias\cegui-0.8.7\dependencies\bin\pcre_d.dll exes
Xcopy Dependencias\cegui-0.8.7\dependencies\bin\glew_d.dll exes
Xcopy Dependencias\cegui-0.8.7\dependencies\bin\glfw_d.dll exes
Xcopy Dependencias\cegui-0.8.7\dependencies\bin\jpeg_d.dll exes
Xcopy Dependencias\cegui-0.8.7\dependencies\bin\libexpat_d.dll exes
Xcopy Dependencias\cegui-0.8.7\dependencies\bin\libpng_d.dll exes
Xcopy Dependencias\cegui-0.8.7\dependencies\bin\pcre_d.dll exes
Xcopy Dependencias\cegui-0.8.7\dependencies\bin\SILLY_d.dll exes
Xcopy Dependencias\cegui-0.8.7\dependencies\bin\zlib_d.dll exes

rem Librerías dinámicas para OIS
Xcopy Dependencias\OIS-1.5\Build\Debug\OIS_d.dll exes
Xcopy Dependencias\OIS-1.5\Build\Release\OIS.dll exes

rem Librerías dinámicas para FMOD
Xcopy Dependencias\fmod\fsbank\lib\fsbank.dll exes
Xcopy Dependencias\fmod\fsbank\lib\fsbank64.dll exes
Xcopy Dependencias\fmod\fsbank\lib\libfsbvorbis.dll exes
Xcopy Dependencias\fmod\fsbank\lib\libfsbvorbis64.dll exes
Xcopy Dependencias\fmod\lowlevel\lib\fmod.dll exes
Xcopy Dependencias\fmod\lowlevel\lib\fmod64.dll exes
Xcopy Dependencias\fmod\lowlevel\lib\fmodL.dll exes
Xcopy Dependencias\fmod\lowlevel\lib\fmodL64.dll exes
Xcopy Dependencias\fmod\studio\lib\fmodstudio.dll exes
Xcopy Dependencias\fmod\studio\lib\fmodstudio64.dll exes
Xcopy Dependencias\fmod\studio\lib\fmodstudioL.dll exes
Xcopy Dependencias\fmod\studio\lib\fmodstudioL64.dll exes

rem Librerías dinámicas para VLD
XCopy Dependencias\vld-2.5.1\bin\Win64\vld_x64.dll exes
XCopy Dependencias\vld-2.5.1\bin\Win64\dbghelp.dll exes
XCopy Dependencias\vld-2.5.1\bin\Win64\Microsoft.DTfW.DHL.manifest exes


Xcopy tmp\* exes

del /q tmp
rmdir tmp



