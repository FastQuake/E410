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
player.height = 6
player.flying = false
player.shipRot = Vector.create(0,0,0)
function createObject(obj)
	if obj:getTag() == "player"..player.id then
		player.model = obj
		player.model:setVisible(false)
	end
end

function onReceivePacket(data)
	if data[1]:sub(1,6) == "player" then	
		player.id = data[1]:sub(7)
	elseif data[1] == "shiprot" then
		player.shipRot.x = data[2]
		player.shipRot.y = data[3]
		player.shipRot.z = data[4]
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
		if player.flying == false then
			cam:turn(mousex*sensitvity,
			-mousey*sensitvity)
		else
			cam:setRot(-player.shipRot.y, player.shipRot.x, player.shipRot.z)
		end
		input.setMousePos(width/2, height/2)
		player.newRot = player.oldRot-mousex*sensitvity
		if player.newRot ~= player.oldRot then
			network.sendPacket("turn "..mousex*sensitvity.." "..-mousey*sensitvity)
		end
		player.oldRot = player.newRot
	end
end
