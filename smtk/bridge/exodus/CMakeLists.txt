# Only include the VTK use-file in subdirectories
# that require VTK include paths. Otherwise, command
# lines get too long on some platforms.
include("${VTK_USE_FILE}")
  list(APPEND CMAKE_MODULE_PATH "${VTK_MODULES_DIR}")

set(exodusSrcs
  Session.cxx
  Operator.cxx
  ReadOperator.cxx
)

set(exodusHeaders
  Session.h
  Operator.h
  ReadOperator.h
)

install(FILES PointerDefs.h DESTINATION include/smtk/bridge/exodus)

add_library(SMTKSessionExodus ${exodusSrcs})

#set SMTKSessionExodus to publicly link to SMTKCore and VTK
target_link_libraries(SMTKSessionExodus
  LINK_PUBLIC
    SMTKCore
  LINK_PRIVATE
    vtkIOExodus
    vtkIOParallelExodus
    vtkFiltersGeometry
    vtkCommonDataModel
    vtksys
)
smtk_export_header(SMTKSessionExodus SMTKSessionExodusExports.h)

# Add compile definitions and include directories
#set_property(TARGET SMTKSessionExodus APPEND PROPERTY COMPILE_FLAGS ${EXODUS_DEFINES})
#set_property(TARGET SMTKSessionExodus APPEND PROPERTY INCLUDE_DIRECTORIES ${EXODUS_INCLUDE_DIRS})

# On Mac OS X, set the directory included as part of the installed library's path:
if (BUILD_SHARED_LIBS)
  set_target_properties(SMTKSessionExodus PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
endif()

#install the library and exports the library when used from a build tree
smtk_install_library(SMTKSessionExodus DEPENDS SMTKCore)

# Operators which have XML descriptions in separate files
# need to have it encoded as a string in a header.
# We do not need the path to the generated header (appended
# to exodusOperatorXML) since the operators themselves include
# the header in their implementations.
smtk_operator_xml("${CMAKE_CURRENT_SOURCE_DIR}/ReadOperator.sbt" exodusOperatorXML)
smtk_session_json("${CMAKE_CURRENT_SOURCE_DIR}/Session.json" exodusSessionJSON)

# Install the headers
smtk_public_headers(${exodusHeaders})

if(SMTK_BUILD_PYTHON_WRAPPINGS AND Shiboken_FOUND)
  #extract the headers from exodus library we built to give them to shiboken

  sbk_wrap_library(SMTKSessionExodus
    GENERATOR_ARGS --avoid-protected-hack
    WORKING_DIRECTORY ${SMTK_SOURCE_DIR}/smtk
    LOCAL_INCLUDE_DIRECTORIES
      ${SMTK_SOURCE_DIR}/smtk/common
      ${SMTK_SOURCE_DIR}/smtk/attribute
      ${SMTK_SOURCE_DIR}/smtk/model
      ${SMTK_SOURCE_DIR}/smtk/session
      ${SMTK_SOURCE_DIR}/smtk/bridge/exodus
      ${SMTK_SOURCE_DIR}/smtk/simulation
      ${SMTK_SOURCE_DIR}/smtk/io
      ${SMTK_SOURCE_DIR}/smtk/view
      ${SMTK_SOURCE_DIR}/smtk
      ${SMTK_BINARY_DIR}/smtk
      ${CMAKE_CURRENT_BINARY_DIR}
    TYPESYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/typesystem.xml
    HEADERS ${exodusHeaders}
    DEPENDS SMTKCore
  )
target_include_directories(SMTKSessionExodusPython
    PRIVATE ${EXODUS_INCLUDE_DIRS})
endif()

if(SMTK_BUILD_SESSION_PLUGIN)
  add_subdirectory(plugin)
endif()

if (SMTK_ENABLE_TESTING)
  # ... and make header compilation tests link properly:
  target_link_libraries(TestBuild_smtk_bridge_exodus
    LINK_PRIVATE
      vtkIOExodus
      vtkIOParallelExodus
      vtkFiltersGeometry
      vtkCommonDataModel
      vtksys
  )
  add_subdirectory(testing)
endif()
