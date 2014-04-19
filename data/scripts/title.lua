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
		title.state = tstates.play
	end)

	title.multi = GUI.createButton()
	title.multi:setString("Multiplayer")
	title.multi:setCharSize(24)
	title.multi:setPos(10,-125)
	title.multi:setCallback(function()
		print("Multiplayer time!")
		title.state = tstates.multi
	end)

	title.settings = GUI.createButton()
	title.settings:setString("Settings")
	title.settings:setCharSize(24)
	title.settings:setPos(10,-100)
	title.settings:setCallback(function()
		print("Settings time!")
		title.state = tstates.settings
	end)

	title.credits = GUI.createButton()
	title.credits:setString("Credits")
	title.credits:setCharSize(24)
	title.credits:setPos(10,-75)
	title.credits:setCallback(function()
		print("Credits time!")
		title.state = tstates.credits
	end)

	title.quit = GUI.createButton()
	title.quit:setString("Quit")
	title.quit:setCharSize(24)
	title.quit:setPos(10,-50)
	title.quit:setCallback(function()
		print("Quittin time!")
		quit()
	end)
	return title
end

function Title:update(dt)
end
