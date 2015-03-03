# **************************************************************************
# * Copyright(c) 1998-2014, ALICE Experiment at CERN, All rights reserved. *
# *                                                                        *
# * Author: The ALICE Off-line Project.                                    *
# * Contributors are mentioned in the code where appropriate.              *
# *                                                                        *
# * Permission to use, copy, modify and distribute this software and its   *
# * documentation strictly for non-commercial purposes is hereby granted   *
# * without fee, provided that the above copyright notice appears in all   *
# * copies and that both the copyright notice and this permission notice   *
# * appear in the supporting documentation. The authors make no claims     *
# * about the suitability of this software for any purpose. It is          *
# * provided "as is" without express or implied warranty.                  *
# **************************************************************************

# Module
set(MODULE EveBase)

# Module include folder
include_directories(${AliRoot_SOURCE_DIR}/EVE/${MODULE})

# Additional include folders in alphabetical order
include_directories(
                    ${AliRoot_SOURCE_DIR}/ANALYSIS/ANALYSIS
                    ${AliRoot_SOURCE_DIR}/ANALYSIS/ANALYSISalice
                    ${AliRoot_SOURCE_DIR}/ITS/ITSbase
                    ${AliRoot_SOURCE_DIR}/ITS/ITSrec
                    ${AliRoot_SOURCE_DIR}/MONITOR/alionlinereco
                    ${AliRoot_SOURCE_DIR}/MONITOR/alistoragemanager
                    ${AliRoot_SOURCE_DIR}/MONITOR/MONITORzmq
                    ${AliRoot_SOURCE_DIR}/RAW/RAWDatabase
                    ${AliRoot_SOURCE_DIR}/RAW/RAWDatarec
                    ${AliRoot_SOURCE_DIR}/STEER/AOD
                    ${AliRoot_SOURCE_DIR}/STEER/CDB
                    ${AliRoot_SOURCE_DIR}/STEER/ESD
                    ${AliRoot_SOURCE_DIR}/STEER/STEER
                    ${AliRoot_SOURCE_DIR}/STEER/STEERBase
                    ${AliRoot_SOURCE_DIR}/TPC/TPCbase
                    ${AliRoot_SOURCE_DIR}/TPC/TPCrec
                    ${AliRoot_SOURCE_DIR}/TPC/TPCsim
                   )

# Sources in alphabetical order
set(SRCS
    AliEveApplication.cxx
    AliEveCascade.cxx
    AliEveCascadeEditor.cxx
    AliEveCascadeListEditor.cxx
    AliEveConfigManager.cxx
    AliEveCosmicRayFitter.cxx
    AliEveCosmicRayFitterEditor.cxx
    AliEveEventManager.cxx
    AliEveEventManagerEditor.cxx
    AliEveEventSelector.cxx
    AliEveEventSelectorWindow.cxx
    AliEveFileDialog.cxx
    AliEveGedEditor.cxx
    AliEveHF.cxx
    AliEveHFEditor.cxx
    AliEveHFListEditor.cxx
    AliEveJetPlane.cxx
    AliEveJetPlaneEditor.cxx
    AliEveJetPlaneGL.cxx
    AliEveKineTools.cxx
    AliEveKink.cxx
    AliEveKinkEditor.cxx
    AliEveKinkListEditor.cxx
    AliEveMacro.cxx
    AliEveMacroEditor.cxx
    AliEveMacroExecutor.cxx
    AliEveMacroExecutorWindow.cxx
    AliEveMagField.cxx
    AliEveMainWindow.cxx
    AliEveMultiView.cxx
    AliEveTrackCounter.cxx
    AliEveTrackCounterEditor.cxx
    AliEveTrack.cxx
    AliEveTrackFitter.cxx
    AliEveTrackFitterEditor.cxx
    AliEveTracklet.cxx
    AliEveUtil.cxx
    AliEveV0.cxx
    AliEveV0Editor.cxx
    AliEveV0ListEditor.cxx
    AliEveVSDCreator.cxx
   )

# Headers from sources
string(REPLACE ".cxx" ".h" HDRS "${SRCS}")

# Generate the dictionary
# It will create G_ARG1.cxx and G_ARG1.h / ARG1 = function first argument
get_directory_property(incdirs INCLUDE_DIRECTORIES)
generate_dictionary("${MODULE}" "${MODULE}LinkDef.h" "${HDRS}" "${incdirs}")

# Generate the ROOT map
# Dependecies
set(LIBDEPS STEERBase STEER ESD CDB AOD RAWDatabase RAWDatarec ANALYSIS ANALYSISalice ITSbase ITSrec TPCbase TPCrec TPCsim EG Eve Ged Gpad Graf Graf3d Gui Minuit Net RGL Rint)
generate_rootmap("${MODULE}" "${LIBDEPS}" "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}LinkDef.h")

# Add a library to the project using the specified source files
add_library(${MODULE} SHARED ${SRCS} G__${MODULE}.cxx)
target_link_libraries(${MODULE} STEERBase STEER ESD CDB AOD RAWDatabase RAWDatarec ANALYSIS ANALYSISalice ITSbase ITSrec TPCbase TPCrec TPCsim EG Eve Ged Gpad Graf Graf3d Gui Minuit Net RGL Rint)

# Additional compilation flags
set_target_properties(${MODULE} PROPERTIES COMPILE_FLAGS "")

# System dependent: Modify the way the library is build
if(${CMAKE_SYSTEM} MATCHES Darwin)
    set_target_properties(${MODULE} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(${CMAKE_SYSTEM} MATCHES Darwin)

# Installation
install(TARGETS ${MODULE}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

install(FILES ${HDRS} DESTINATION include)