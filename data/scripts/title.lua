Title = {}
Title.__index = Title

function Title.create()
	--Do meta table stuff
	local title = {}
	setmetatable(title,Title)
	--Create GUI elements
	title.title = GUI.createText()
	title.title:setString("E410")
	title.title:setCharSize(48)
	title.title:setPos(10,10)

	title.play = GUI.createButton()
	title.play:setString("Play")
	title.play:setCharSize(24)
	title.play:setPos(10,-125)

	title.multi = GUI.createButton()
	title.multi:setString("Multiplayer")
	title.multi:setCharSize(24)
	title.multi:setPos(10,-100)

	title.settings = GUI.createButton()
	title.settings:setString("Settings")
	title.settings:setCharSize(24)
	title.settings:setPos(10,-75)

	title.quit = GUI.createButton()
	title.quit:setString("Quit")
	title.quit:setCharSize(24)
	title.quit:setPos(10,-50)
	return title
end

function Title:update(dt)
end
