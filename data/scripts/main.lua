require "title"
require "vector"
require "hud"
require "networkutils"
require "ai"
require "bullet"

sensitvity = 0.75
speed = 10

states = {
	default = 0,
	title = 1,
	play = 2,
	server = 3,
}

state = states.default
player = {}
player.model = nil
player.id = -1
player.height = 2
player.hp = 100
player.ammo = 100
paused = false
pausetimer = 0
bullets = {}
bulletTimer = 0
function myPos()
	print(Vector.create(player.model:getPos()))
end

function createObject(obj)
	if obj:getTag() == "player"..player.id then
		player.model = obj
		player.model:setVisible(false)
		player.model:animate(true)
	end
	
	if obj:getTag():sub(1,6) == "player" then
		obj:animate(true)
		player.model:setOrigin(0,0,0)
	elseif obj:getTag() == "monster" then
		obj:animate(true)
	end
end

function onReceivePacket(data)
	if data[1]:sub(1,6) == "player" then	
		player.id = data[1]:sub(7)
		h:show(true)
	elseif data[1] == "animate" then
		local obj = getModelFromID(tonumber(data[2]))
		if obj ~= nil then
			if obj:getCurAnim() ~= "walk" then
				obj:setCurAnim("walk")
				obj:animate(true)
			end
		end
	elseif data[1] == "stopanimate" then
		local obj = getModelFromID(tonumber(data[2]))
		if obj ~= nil then
			if obj:getCurAnim() ~= "stand" then
				obj:setCurAnim("stand")
				obj:animate(true)
			end
		end
	elseif data[1] == "shoot" then
		table.insert(bullets,Bullet.create(Vector.create(data[2],data[3],data[4]),Vector.create(data[5],data[6],data[7])))
	elseif data[1] == "hit" then
		player.hp = player.hp - 10
	end
end

function onServerDisconnect()
	print("BYE BYE!")
	state = states.title
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
	elseif key == keys.Escape then
		title:setPause(true)
		paused = true
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
	--fpsCounter:setVisible(false)
	--Create stuff for scene
	cam = camera.createCam()
	cam:setPos(0,0,0)
	cam:setRot(0,0,0)
	camera.setCam(cam)

	h = HUD.create(player)
	h:show(false)

	s = GO.createSprite("explosion.png")
	s:setFrameSize(16,1)
	s:animate(true)

	planetLight = GO.createLight()
	planetLight:setPos(-3.5,0,5)
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
	bulletTimer = bulletTimer+dt
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

		GO.deleteLight(planetLight)

		light = GO.createLight()
		light:setPos(18,2.35,2.55)
		light2 = GO.createLight()
		light2:setPos(0,14,-4)
		light2:setIntensity(550)

		sun = GO.createLight()
		sun:setPos(0,20,0)
		sun:setIntensity(8000)

		light2 = GO.createLight()
		light2:setPos(16,2.35,-12.55)
	end
	if state == states.play then
		h:update()
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
		if paused then
			pausetimer = pausetimer+dt
		end
		if input.isGuiKeyDown(keys.Escape) and pausetimer > 0.5 then
			title:setPause(false)
			pausetimer = 0
			paused = false
			input.setGuiMousePos(width/2,height/2)
		end
		if input.isMouseDown(mouse.Left) and bulletTimer > 0.3 and player.ammo > 0 then
			local pos = Vector.create(cam:getPos())
			local dir = Vector.create(cam:getLookat())
			network.sendPacket("shoot "..(pos+dir).." "..dir)
			player.ammo = player.ammo -1
			bulletTimer = 0
		end
		player.oldRot = player.newRot

		for k,v in pairs(bullets) do
			v.pos = v.pos + Vector.scalarMul(100*dt,v.dir)
			v.sprite:setPos(v.pos:get())
			local p1 = (v.pos - v.spos):mag()
			local p2 = (v.stop - v.spos):mag()
			if p1 > p2 then
				v.sprite:setVisible(false)
				table.remove(bullets,k)
			end
		end
	elseif state == states.title then
		h:show(false)
		title:show(true)
		cam:setPos(0,0,0)
		cam:setRot(0,0,0)
		state = states.default
	end
end
