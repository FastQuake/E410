require "title"
require "vector"
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
player.height = 9
function createObject(obj)
	if obj:getTag() == "player"..player.id then
		player.model = obj
		--player.model:setVisible(false)
	end
end

function onReceivePacket(data)
	if data[1]:sub(1,6) == "player" then	
		player.id = data[1]:sub(7)
	end
end

function onKeyDown(key)
	if key == keys.W then
		network.sendPacket("forward")
	elseif key == keys.S then
		network.sendPacket("backward")
	elseif key == keys.A then
		network.sendPacket("left")
	elseif key == keys.D then
		network.sendPacket("right")
	elseif key == keys.P then
		local pos = Vector.create(cam:getPos())
		local dir = Vector.create(cam:getLookat())
		network.sendPacket("cast "..(pos+dir).." "..dir)
	end
end

function onKeyRelease(key)
	if key == keys.W or key == keys.S or key == keys.A or key == keys.D then
		network.sendPacket("stop")
	end
end

function init()
	title = Title.create()
	cam = camera.createCam()
	cam:setPos(0,0,0)
	cam:setRot(0,0,0)
	camera.setCam(cam)
end

player.oldRot = 0
player.newRot = 0
function update(dt)
	title:update(dt)
	if title.state == tstates.play then
		title:show(false)
		title.state = tstates.default
		state = states.play
		--Start local server and do whatever here
		cam:setPos(0,2,0)
		--cam:setRot(0,90,0)
		cam:setRot(0,0,0)
		input.setGuiMousePos(width/2, height/2)
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
		cam:turn(mousex*sensitvity,
		-mousey*sensitvity)
		input.setMousePos(width/2, height/2)
		player.newRot = player.oldRot-mousex*sensitvity
		if player.newRot ~= player.oldRot then
			network.sendPacket("turn "..player.newRot)
		end
		player.oldRot = player.newRot
	end
end
