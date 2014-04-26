require "title"
sensitvity = 1
speed = 10

states = {
	title = 0,
	play = 1,
	server = 2,
}

state = states.title
local player = {}
function createObject(obj)
	if obj:getTag() == "player"..player.id then
		player.model = obj
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
	camera.setCam(cam)
end

function update(dt)
	title:update(dt)
	if title.state == tstates.play then
		title:show(false)
		title.state = tstates.default
		state = states.play
		--Start local server and do whatever here
		network.startServer()
		network.connectTo("localhost")
		cam:setPos(0,2,0)
		cam:setRot(0,110,0)
	end
	if state == states.play then
		if player.model ~= nil then
			x,y,z = player.model:getPos()
			cam:setPos(x-10,y+10,z)
		end
		if input.isKeyDown(keys.Up) then
			network.sendPacket("forward")
		end
		if input.isKeyDown(keys.Right) then
			network.sendPacket("right")
		end
		if input.isKeyDown(keys.Left) then
			network.sendPacket("left")
		end
	end
end
