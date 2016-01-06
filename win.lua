print("start")

local function MainLoop()
	ShowFps()
end

local function Boot(name, width, height)
	RunApp(name, width, height)
	if InitOpenGL() then
		print("Run OpenGL Mode")
		while ProcMsg() do
			MainLoop()
			Ping()
		end
	end
	TermOpenGL()
end

Boot("LUA TEST", 1280, 720)

