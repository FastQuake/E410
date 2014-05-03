require "title"
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
player.height = 8
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

function init()
	title = Title.create()
	cam = camera.createCam()
	cam:setPos(0,0,0)
	cam:setRot(0,0,0)
	camera.setCam(cam)
end

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
			x,y,z = player.model:getPos()
			cam:setPos(x,y+player.height,z)
		end
		local mousex, mousey = input.getMousePos()
		mousex = mousex - (width/2)
		mousey = mousey - (height/2)
		cam:turn(mousex*sensitvity,
		-mousey*sensitvity)
		input.setMousePos(width/2, height/2)

		local camx,camy,camz = cam:getRot()
		dir = camx.." "..camy
		if input.isKeyDown(keys.W) then
			network.sendPacket("forward "..dir)
		end
		if input.isKeyDown(keys.S) then
			network.sendPacket("backward "..dir)
		end
		if input.isKeyDown(keys.A) then
			network.sendPacket("left "..dir)
		end
		if input.isKeyDown(keys.D) then
			network.sendPacket("right "..dir)
		end
		--[[if input.isKeyDown(keys.Up) then
			network.sendPacket("forward")
		end
		if input.isKeyDown(keys.Right) then
			network.sendPacket("right")
		end
		if input.isKeyDown(keys.Left) then
			network.sendPacket("left")
		end]]--

	end
end
