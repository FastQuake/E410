require "title"
require "vector"
require "hud"
sensitvity = 0.75
speed = 10

states = {
	title = 0,
	play = 1,
	server = 2,
}

state = states.title
player = {}
player.model = nil
player.id = -1
player.height = 2
player.hp = 100
function createObject(obj)
	if obj:getTag() == "player"..player.id then
		player.model = obj
		player.model:setVisible(false)
	end
end

function onReceivePacket(data)
	if data[1]:sub(1,6) == "player" then	
		player.id = data[1]:sub(7)
		h:show(true)
	end
end

function onKeyDown(key)
	if key == keys.W then
		network.sendPacket("move forward")
	elseif key == keys.S then
		network.sendPacket("move backward")
	elseif key == keys.A then
		network.sendPacket("move left")
	elseif key == keys.D then
		network.sendPacket("move right")
	elseif key == keys.E then
		local pos = Vector.create(cam:getPos())
		local dir = Vector.create(cam:getLookat())
		network.sendPacket("cast "..(pos+dir).." "..dir)
	elseif key == keys.Space then
		network.sendPacket("jump")
	end
end

function onKeyRelease(key)
	if key == keys.W then
		network.sendPacket("stop forward")
	elseif key == keys.S then
		network.sendPacket("stop backward")
	elseif key == keys.A then
		network.sendPacket("stop left")
	elseif key == keys.D then
		network.sendPacket("stop right")
	end
end

function init()
	title = Title.create()
	--Create stuff for GUI
	fpsCounter = GUI.createText()
	fpsCounter:setCharSize(26)
	fpsCounter:setString("FPS: ".. 0)
	fpsCounter:setVisible(false)
	--Create stuff for scene
	cam = camera.createCam()
	cam:setPos(0,0,0)
	cam:setRot(0,0,0)
	camera.setCam(cam)

	light = GO.createLight()
	light:setPos(16,2.35,2.55)

	h = HUD.create(player)
	h:show(false)
end

player.oldRot = 0
player.newRot = 0

frames = 0
time = 0

function update(dt)
	if time > 1 then
		fpsCounter:setString("FPS: "..frames)
		frames = 0
		time = 0
	end
	time = time + dt
	frames = frames + 1
	title:update(dt)
	h:update()
	if title.state == tstates.play then
		title:show(false)
		title.state = tstates.default
		state = states.play
		--Start local server and do whatever here
		cam:setPos(0,2,0)
		--cam:setRot(0,90,0)
		cam:setRot(0,0,0)
		input.setGuiMousePos(width/2, height/2)
		--lamp = GO.createLight("point")
		--lamp:setPos(18,2.35,2.55)
	end
	if state == states.play then
		if player.model ~= nil then
			pos = Vector.create(player.model:getPos())
			pos = pos+Vector.create(0,player.height,0)
			cam:setPos(pos:get())
		end
		local mousex, mousey = input.getMousePos()
		mousex = mousex - (width/2)
		mousey = mousey - (height/2)
		cam:turn(-mousey*sensitvity,
		mousex*sensitvity)
		input.setMousePos(width/2, height/2)
		player.newRot = player.oldRot-mousex*sensitvity
		if player.newRot ~= player.oldRot then
			network.sendPacket("turn "..mousex*sensitvity.." "..-mousey*sensitvity)
		end
		player.oldRot = player.newRot
	end
end
