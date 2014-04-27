sensitvity = 10
speed = 10

function createObject(obj)
end

function init()
	--Create stuff for GUI
	fpsCounter = GUI.createText()
	fpsCounter:setCharSize(26)
	fpsCounter:setString("FPS: ".. 0)
	--Create stuff for scene
	cam = ent.createCam()
	cam:setPos(-2, 6, 2)
	ent.setCam(cam)

	fixit = ent.loadIQM("mr_fixit.iqm")
	floor = ent.loadIQM("cube.iqm")
	shit = ent.loadIQM("monkey.iqm")
	cube = ent.loadIQM("cube.iqm")

	floor:setScale(20, 0.1, 20)
	floor:setPos(0, 0, 0)
	shit:setPos(6, 1.1, 0)
	cube:setScale(1.5,1.5,1.5)
	cube:setPos(-3,1,2)
end

time = 0
frames = 0;

function update(dt)
	time = time + dt
	frames = frames + 1
	if time > 1.0 then
		fpsCounter:setString("FPS: " .. frames)
		frames = 0
		time = 0
	end
	--Move camera with standard FPS controls
	local mousex, mousey = input.getMousePos()
	mousex = mousex - (width/2)
	mousey = mousey - (height/2)
	cam:turn(mousey*sensitvity*dt, 
		mousex*sensitvity*dt)
	input.setMousePos(width/2, height/2)

	if input.isKeyDown(keys.W) then 
		cam:move(speed*dt)
	end 
	if input.isKeyDown(keys.S) then
		cam:move(-speed*dt)
	end
	if input.isKeyDown(keys.A) then
		cam:strafe(-speed*dt)
	end
	if input.isKeyDown(keys.D) then
		cam:strafe(speed*dt)
	end
	if input.isKeyDown(keys.J) then
		network.sendPacket("forward")
	end
	if input.isKeyDown(keys.H) then
		network.sendPacket("turn left")
	end
	if input.isKeyDown(keys.L) then
		network.sendPacket("turn right")
	end
end
