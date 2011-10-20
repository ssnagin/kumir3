macro(handleTranslation pluginName)
    file(GLOB ts_files ${CMAKE_SOURCE_DIR}/share/kumir2/translations/${pluginName}_??.ts)
    foreach(ts_file IN ITEMS ${ts_files})
        file(MAKE_DIRECTORY ${SHARE_PATH}/translations/)
        get_filename_component(basename ${ts_file} NAME_WE)
        set(qm_target ${basename}_qm)
        #add_custom_target(${qm_target} ALL ${QT_LRELEASE_EXECUTABLE} -qm ${SHARE_PATH}/translations/${basename}.qm ${ts_file})
        add_custom_command(TARGET ${pluginName} PRE_BUILD
                COMMAND ${QT_LRELEASE_EXECUTABLE} -qm ${SHARE_PATH}/translations/${basename}.qm ${ts_file}
                #COMMAND echo aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
        )
        install(FILES ${SHARE_PATH}/translations/${basename}.qm DESTINATION ${RESOURCES_DIR}/translations/)
    endforeach(ts_file)
endmacro(handleTranslation)
