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
	--Create GUI Text and Buttons
	title.title = GUI.createText()
	title.title:setString("E410")
	title.title:setCharSize(48)
	title.title:setPos(10,10)

	title.play = GUI.createButton()
	title.play:setString("Play")
	title.play:setCharSize(24)
	title.play:setPos(10,-150)
	title.play:setCallback(function()
		print("Play time!")
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
	title.multi:setCallback(function()
		print("Multiplayer time!")
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
	title.settings:setCallback(function()
		print("Settings time!")
		if title.state == tstates.settings then
			title.state = tstates.default
		else
			title.state = tstates.settings
		end
	end)

	title.credits = GUI.createButton()
	title.credits:setString("Credits")
	title.credits:setCharSize(24)
	title.credits:setPos(10,-75)
	title.credits:setCallback(function()
		print("Credits time!")
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
	title.quit:setCallback(function()
		print("Quittin time!")
		quit()
	end)

	--Create credits window
	title.creds = {}
	title.creds.box = GUI.createBox()
	title.creds.box:setScale(400,400)
	title.creds.box:setPos(-410,-410)
	title.creds.box:setColour(100,100,100,255)
	title.creds.box:setVisible(false)

	title.creds.title = GUI.createText()
	title.creds.title:setString("Credits")
	title.creds.title:setCharSize(32)
	title.creds.title:setPos(-380,-400)

	title.creditText = [[
	Lucas Fryzek - 
	 Programmer and Artist
	John Brooks -
	 Programmer and Artist
	
	Special Thanks to Astrid Fryzek 
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
	return title
end

function Title:setCredits(bool)
	for key,value in pairs(self.creds) do
		value:setVisible(bool)
	end
end

function Title:update(dt)
	if self.state == tstates.credits then
		self:setCredits(true)
	else
		self:setCredits(false)
	end
end
