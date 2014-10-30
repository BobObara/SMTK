
include_directories(${CGM_INCLUDE_DIRS})

set(cgmSrcs
  Bridge.cxx
  CAUUID.cxx
  Engines.cxx
  ExportSolid.cxx
  ImportSolid.cxx
  Operator.cxx
  ReadOperator.cxx
  CreateSphereOperator.cxx
  TDUUID.cxx
)

set(cgmHeaders
  Bridge.h
  CAUUID.h
  Engines.h
  ExportSolid.h
  ImportSolid.h
  Operator.h
  ReadOperator.h
  CreateSphereOperator.h
  TDUUID.h
)

install(FILES PointerDefs.h DESTINATION include/smtk/bridge/cgm)

add_definitions(${CGM_DEFINES})

add_library(cgmSMTK ${cgmSrcs})

#set cgmSMTK to publicly link to SMTKCore and CGM
target_link_libraries(cgmSMTK
  LINK_PUBLIC
    SMTKCore
  LINK_PRIVATE
    ${CGM_LIBRARIES}
  )
smtk_export_header(cgmSMTK cgmSMTKExports.h)

# Add compile definitions and include directories
set_property(TARGET cgmSMTK APPEND PROPERTY COMPILE_FLAGS ${CGM_DEFINES})
set_property(TARGET cgmSMTK APPEND PROPERTY INCLUDE_DIRECTORIES ${CGM_INCLUDE_DIRS})

# On Mac OS X, set the directory included as part of the installed library's path:
if (BUILD_SHARED_LIBS)
  set_target_properties(cgmSMTK PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
endif()

#install the library and exports the library when used from a build tree
smtk_install_library(cgmSMTK DEPENDS SMTKCore)

# Normally we would uncomment this line:
#    smtk_bridge_json("${CMAKE_CURRENT_SOURCE_DIR}/Bridge.json" cgmBridgeJSON)
# to generate a header file containing the JSON string defining
# tags for the CGM bridge. However, because we wish to omit portions
# of the JSON depending on configuration options, a hand-edited header
# is included in the source tree instead. The other option would be to
# use nodeJS or Python to conditionally create the JSON and/or header.
# But this is simpler.

# Operators which have XML descriptions in separate files
# need to have it encoded as a string in a header.
# We do not need the path to the generated header (appended
# to cgmOperatorXML) since the operators themselves include
# the header in their implementations.
smtk_operator_xml("${CMAKE_CURRENT_SOURCE_DIR}/ReadOperator.sbt" cgmOperatorXML)
smtk_operator_xml("${CMAKE_CURRENT_SOURCE_DIR}/CreateSphereOperator.sbt" cgmOperatorXML)

# Install the headers
smtk_public_headers(${cgmHeaders})

# Utility programs using CGM:
add_executable(cgm-convert cgm-convert.cxx)
target_link_libraries(cgm-convert SMTKCore cgmSMTK ${CGM_LIBRARIES})

add_executable(facet-convert facet-convert.cxx)
target_link_libraries(facet-convert SMTKCore ${CGM_LIBRARIES})

if (SMTK_BUILD_VTK)
  include("${VTK_USE_FILE}")
  add_executable(vtk-to-cholla vtk-to-cholla.cxx)
  target_link_libraries(vtk-to-cholla
    SMTKCore
    vtkCommonDataModel
    vtkIOXML
    ${CGM_LIBRARIES}
  )
endif()


if(SMTK_BUILD_PYTHON_WRAPPINGS AND Shiboken_FOUND)
  #extract the headers from cgm library we built to give them to shiboken

  sbk_wrap_library(cgmSMTK
    GENERATOR_ARGS --avoid-protected-hack
    WORKING_DIRECTORY ${SMTK_SOURCE_DIR}/smtk
    LOCAL_INCLUDE_DIRECTORIES
      ${SMTK_SOURCE_DIR}/smtk/common
      ${SMTK_SOURCE_DIR}/smtk/attribute
      ${SMTK_SOURCE_DIR}/smtk/model
      ${SMTK_SOURCE_DIR}/smtk/bridge
      ${SMTK_SOURCE_DIR}/smtk/bridge/cgm
      ${SMTK_SOURCE_DIR}/smtk/simulation
      ${SMTK_SOURCE_DIR}/smtk/io
      ${SMTK_SOURCE_DIR}/smtk/view
      ${SMTK_SOURCE_DIR}/smtk
      ${SMTK_BINARY_DIR}/smtk
      ${CMAKE_CURRENT_BINARY_DIR}
    TYPESYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/typesystem_cgm.xml
    HEADERS ${cgmHeaders}
    DEPENDS SMTKCore
  )
  target_include_directories(cgmSMTKPython
    PRIVATE ${CGM_INCLUDE_DIRS})

    configure_file(
      "${CMAKE_CURRENT_SOURCE_DIR}/cgmsmtk.py"
      "${SMTK_BINARY_DIR}/cgmsmtk.py"
      @ONLY
    )

  #todo we need to install this file in such a way that it can resolve
  #the location of shiboken, which is going to mean running fixup bundle
  #on the mac, and than fixing up the cgmsmtk.py package script

  install(CODE "set(LIBRARY_OUTPUT_PATH \"${CMAKE_INSTALL_PREFIX}/lib\")
                configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/cgmsmtk.py ${CMAKE_INSTALL_PREFIX}/python/cgmsmtk.py )" )
endif()

if(SMTK_BUILD_BRIDGE_PLUGIN AND SMTK_BUILD_CGM)
  add_subdirectory(plugin)
endif()

if (SMTK_ENABLE_TESTING)
  # ... and make header compilation tests link properly:
  target_link_libraries(TestBuild_smtk_bridge_cgm LINK_PRIVATE ${CGM_LIBRARIES})
  add_subdirectory(testing)
endif()