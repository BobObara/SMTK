# Add the Qt GUI code if enabled
if(SMTK_BUILD_QT)
  add_subdirectory(qt)
endif()

# Add mesh operator if Remus is enabled.
#this needs to be above vtk_smtk_setup_module_environment as that
#pollutes the link libraries, and causes the remus plugin to depend
#on libraries it doesn't require
if (SMTK_ENABLE_REMUS)
  add_subdirectory(remus)
endif()

# Add VTK library to render SMTK geometry and interact with ParaView-based apps if enabled.
if(SMTK_BUILD_VTK)
  if(SMTK_BUILD_SESSION_PLUGIN)
    #Only include the ParaView use-file in subdirectories
    # that require ParaView include paths.

    find_package(ParaView REQUIRED)
    include("${PARAVIEW_USE_FILE}")
    pv_setup_module_environment("VTKSMTK")
    pv_process_modules()
  else()
    # Only include the VTK use-file in subdirectories
    # that require VTK include paths. Otherwise, command
    # lines get too long on some platforms.

    # We need the COMPONENTS argument so that this will work
    # with both standalone VTK or Paraview-VTK
    set(__required_vtk_modules
        vtkCommonCore
        vtkCommonDataModel
        vtkCommonExecutionModel
        vtkRenderingCore
        vtkInteractionStyle
        vtkViewsCore
        vtkViewsInfovis
        vtkIOXML
        vtkIOLegacy
        vtkInteractionStyle
        vtkRendering${VTK_RENDERING_BACKEND}
        vtkRenderingVolume${VTK_RENDERING_BACKEND}
        vtkRenderingFreeType${VTK_RENDERING_BACKEND}
    )

    if (SMTK_ENABLE_TESTING)
      list(APPEND __required_vtk_modules vtkTestingRendering)
    endif()
    message("__required_vtk_modules = ${__required_vtk_modules}")
    find_package( VTK REQUIRED COMPONENTS ${__required_vtk_modules} )

    include("${VTK_USE_FILE}")
    list(APPEND CMAKE_MODULE_PATH "${VTK_MODULES_DIR}")
    # message("VTK_MODULES_DIR=${VTK_MODULES_DIR}")

    # Don't add the VTK or other subdirectories as they use the
    # vtk_module_library() macro and vtk/module.cmake file.
    # The pv_process_modules() macro finds all subdirectories
    # containing a module.cmake file automatically.
    include (vtkSMTKModules)
    vtk_smtk_setup_module_environment("VTKSMTK")
    vtk_smtk_process_modules()
  endif()
endif()
