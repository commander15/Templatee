qt_add_qml_module({{ Project.name }}Qml
    URI           {{ Project.name }}
    VERSION       ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    TYPEINFO      {{ Project.name }}
    PLUGIN_TARGET {{ Project.name }}Qml
    CLASS_NAME    {{ Project.name }}QmlPlugin
    NO_GENERATE_PLUGIN_SOURCE
    SOURCES plugin.cpp
)

target_link_libraries({{ Project.name }}Qml PUBLIC  Qt::Qml {{ Project.name }})

# Module installation

qt_query_qml_module({{ Project.name }}Qml
    QMLDIR        DIR
    QML_FILES     FILES
    TYPEINFO      TYPEINFO
    PLUGIN_TARGET TARGET
)

install(FILES ${DIR} ${FILES} ${TYPEINFO} DESTINATION qml/{{ Project.name }})

install(TARGETS ${TARGET}
    ARCHIVE DESTINATION qml/{{ Project.name }}
    LIBRARY DESTINATION qml/{{ Project.name }}
    RUNTIME DESTINATION qml/{{ Project.name }}
)
