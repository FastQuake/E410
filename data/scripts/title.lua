Title = {}
Title.__index = Title

--Title states
tstates = {
	default=0,
	play=1,
	multi=2,
	settings=3,
	credits=4,
}

function Title.create()
	--Do meta table stuff
	local title = {}
	setmetatable(title,Title)
	--Some variable stuff
	title.state = tstates.default
	title.settOptions = {}
	--Create GUI Text and Buttons
	title.title = GUI.createText()
	title.title:setString("E410")
	title.title:setCharSize(48)
	title.title:setPos(10,10)

	title.play = GUI.createButton()
	title.play:setString("Play")
	title.play:setCharSize(24)
	title.play:setPos(10,-150)
	title.play:setBGColour(0,0,0,0)
	title.play:setCallback(function()
		if title.state == tstates.play then
			title.state = tstates.default
		else 
			title.state = tstates.play
		end
	end)

	title.multi = GUI.createButton()
	title.multi:setString("Multiplayer")
	title.multi:setCharSize(24)
	title.multi:setPos(10,-125)
	title.multi:setBGColour(0,0,0,0)
	title.multi:setCallback(function()
		if title.state == tstates.multi then
			title.state = tstates.default
		else
			title.state = tstates.multi
		end
	end)

	title.settings = GUI.createButton()
	title.settings:setString("Settings")
	title.settings:setCharSize(24)
	title.settings:setPos(10,-100)
	title.settings:setBGColour(0,0,0,0)
	title.settings:setCallback(function()
		if title.state == tstates.settings then
			title.state = tstates.default
		else
			title:getOptions()
		end
	end)

	title.credits = GUI.createButton()
	title.credits:setString("Credits")
	title.credits:setCharSize(24)
	title.credits:setPos(10,-75)
	title.credits:setBGColour(0,0,0,0)
	title.credits:setCallback(function()
		if title.state == tstates.credits then
			title.state = tstates.default
		else
			title.state = tstates.credits
		end
	end)

	title.quit = GUI.createButton()
	title.quit:setString("Quit")
	title.quit:setCharSize(24)
	title.quit:setPos(10,-50)
	title.quit:setBGColour(0,0,0,0)
	title.quit:setCallback(function()
		quit()
	end)

	--Create settings window
	title.sett = {}
	title.sett.box = GUI.createBox()
	title.sett.box:setScale(400,400)
	title.sett.box:setPos(-410,-410)
	title.sett.box:setColour(100,100,100,255)
	title.sett.box:setZ(-1)

	title.sett.title = GUI.createText()
	title.sett.title:setString("Settings")
	title.sett.title:setCharSize(32)
	title.sett.title:setPos(-380,-400)

	title.sett.vsync = GUI.createButton()
	title.sett.vsync:setString("VSYNC")
	title.sett.vsync:setCharSize(18)
	title.sett.vsync:setPos(-400,-350)
	title.sett.vsync:setPadding(4)
	title.sett.vsync:setCallback(function()
		title.settOptions.vsync = not title.settOptions.vsync
	end)

	title.sett.fullscreen = GUI.createButton()
	title.sett.fullscreen:setString("Fullscreen")
	title.sett.fullscreen:setCharSize(18)
	title.sett.fullscreen:setPos(-330, -350)
	title.sett.fullscreen:setPadding(4)
	title.sett.fullscreen:setCallback(function()
		title.settOptions.fullscreen = not title.settOptions.fullscreen
	end)

	title.settText = [[
	AA:

	max FPS:

	width:

	height:
	]]

	title.sett.textBoxes = GUI.createText()
	title.sett.textBoxes:setString(title.settText)
	title.sett.textBoxes:setCharSize(18)
	title.sett.textBoxes:setPos(-440, -300)

	title.sett.AA = GUI.createInput()
	title.sett.AA:setCharSize(18)
	title.sett.AA:setPos(-360, -300)

	title.sett.maxFPS = GUI.createInput()
	title.sett.maxFPS:setCharSize(18)
	title.sett.maxFPS:setPos(-300,-264)

	title.sett.width = GUI.createInput()
	title.sett.width:setCharSize(18)
	title.sett.width:setPos(-330,-228)

	title.sett.height = GUI.createInput()
	title.sett.height:setCharSize(18)
	title.sett.height:setPos(-320,-192)

	title.sett.apply = GUI.createButton()
	title.sett.apply:setString("Apply")
	title.sett.apply:setCharSize(18)
	title.sett.apply:setPos(-220,-60)
	title.sett.apply:setBGColour(50,50,50,255)
	title.sett.apply:setPadding(4)
	title.sett.apply:setCallback(function()
		settings.vsync(title.settOptions.vsync)
		settings.fullscreen(title.settOptions.fullscreen)
		settings.AA(title.sett.AA:getInput())
		settings.maxFPS(title.sett.maxFPS:getInput())
		settings.size(title.sett.width:getInput(),title.sett.height:getInput())
		settings.save()

		title:getOptions()
	end)

	--Create multiplayer window
	title.mult = {}
	title.mult.box = GUI.createBox()
	title.mult.box:setScale(400,400)
	title.mult.box:setPos(-410,-410)
	title.mult.box:setColour(100,100,100,255)
	title.mult.box:setZ(-1)

	title.mult.title = GUI.createText()
	title.mult.title:setString("Multiplayer")
	title.mult.title:setCharSize(32)
	title.mult.title:setPos(-380,-400)

	title.mult.iptitle = GUI.createText()
	title.mult.iptitle:setString("Address:")
	title.mult.iptitle:setCharSize(18)
	title.mult.iptitle:setPos(-380,-300)

	title.mult.ipbox = GUI.createInput()
	title.mult.ipbox:setCharSize(18)
	title.mult.ipbox:setPos(-280,-300)

	title.mult.errmsg = GUI.createText()
	title.mult.errmsg:setCharSize(18)
	title.mult.errmsg:setPos(-380,-200)

	title.mult.join = GUI.createButton()
	title.mult.join:setString("Join")
	title.mult.join:setCharSize(18)
	title.mult.join:setPos(-220,-60)
	title.mult.join:setBGColour(50,50,50,255)
	title.mult.join:setPadding(4)
	title.mult.join:setCallback(function()
		title:connect()
	end)

	--Create credits window
	title.creds = {}
	title.creds.box = GUI.createBox()
	title.creds.box:setScale(400,400)
	title.creds.box:setPos(-410,-410)
	title.creds.box:setColour(100,100,100,255)
	title.creds.box:setZ(-1)

	title.creds.title = GUI.createText()
	title.creds.title:setString("Credits")
	title.creds.title:setCharSize(32)
	title.creds.title:setPos(-380,-400)

	title.creditText = [[
	Lucas Fryzek - 
	 Programmer and Artist
	John Brooks -
	 Programmer and Artist
	
	Special Thanks to 
	 Astrid Fryzek & Connor Oliver
	 for artwork

	Technologies used:
	OpenGL - SFML - GLEW
	ENET - Bullet

	Thanks for playing!
	]]

	title.creds.text = GUI.createText()
	title.creds.text:setString(title.creditText)
	title.creds.text:setCharSize(18)
	title.creds.text:setPos(-420,-300)

	--Add planet geometry
	title.planet = GO.loadIQM("planet.iqm")
	title.planet:setPos(3,0,0)
	title.planetspeed = 10
	title.planetangle = 0
	return title
end

function Title:setCredits(bool)
	for key,value in pairs(self.creds) do
		value:setVisible(bool)
	end
end

function Title:setSettings(bool)
	for key,value in pairs(self.sett) do
		value:setVisible(bool)
	end
end

function Title:setMulti(bool)
	for key, value in pairs(self.mult) do
		value:setVisible(bool)
	end
end

function Title:getOptions()
	title.state = tstates.settings
	title.settOptions.vsync = settings.getvsync()
	title.settOptions.fullscreen = settings.getfullscreen()
	title.settOptions.AA = settings.getAA()
	title.settOptions.FPS = settings.getmaxFPS()
	title.settOptions.width, title.settOptions.height = settings.getsize()

	title.sett.AA:setString(title.settOptions.AA)
	title.sett.maxFPS:setString(title.settOptions.FPS)
	title.sett.width:setString(title.settOptions.width)
	title.sett.height:setString(title.settOptions.height)
end

function Title:connect()
	local status = network.connectTo(self.mult.ipbox:getInput())
	if status ~= true then
		self.mult.errmsg:setString(status)
	end
end

function Title:update(dt)
	if self.state == tstates.credits then
		self:setCredits(true)
	else
		self:setCredits(false)
	end

	if self.state == tstates.settings then
		self:setSettings(true)
		--Set colour of vsync button
		if self.settOptions.vsync then
			self.sett.vsync:setBGColour(0,200,0,255)
		else
			self.sett.vsync:setBGColour(255,0,0,255)
		end
		--Set colour of fullscreen button
		if self.settOptions.fullscreen then
			self.sett.fullscreen:setBGColour(0,200,0,255)
		else
			self.sett.fullscreen:setBGColour(255,0,0,255)
		end
	else
		self:setSettings(false)
	end

	if self.state == tstates.multi then
		self:setMulti(true)
		if input.isGuiKeyDown(keys.Return) then
			self:connect()
		end
	else
		self:setMulti(false)
	end

	self.planetangle = self.planetangle + self.planetspeed*dt
	self.planet:setRot(0,self.planetangle,0)
end
