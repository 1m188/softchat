set_project("softchat")

add_rules("mode.debug", "mode.release")

target("client")

    add_rules("qt.widgetapp")
    add_frameworks("QtNetwork", "QtWebSockets")
    add_files("client/**.cpp", "client/**.h")
    add_includedirs("client", "client/gui")

    if is_mode("debug") then
        add_defines("DEBUG")
    elseif is_mode("release") then
        add_defines("RELEASE")
    end

target_end()

target("server")

    add_rules("qt.console")
    add_files("server/**.cpp", "server/**.h")
    add_frameworks("QtNetwork", "QtWebSockets", "QtSql")

    if is_mode("debug") then
        add_defines("DEBUG")
    elseif is_mode("release") then
        add_defines("RELEASE")
    end

target_end()