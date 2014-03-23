sensitvity = 10
speed = 10

function init()
	--Create stuff for GUI
	fpsCounter = GUI.createText()
	GUI.setCharSize(fpsCounter, 26)
	GUI.setString(fpsCounter, "FPS: ".. 0)
	--Create stuff for scene
	cam = camera.createCam()
	camera.setPos(cam, 0, 0, 0)
	camera.setCam(cam)
	fixit = GO.createIQM("mr_fixit.iqm")
	floor = GO.createIQM("cube.iqm")
	shit = GO.createIQM("monkey.iqm")

	GO.setScale(floor,10,0.1,10)
	GO.setPos(floor,0,-0.5,0)
	GO.setPos(shit,5,1.1,0)
end

time = 0
frames = 0;

function update(dt)
	time = time + dt
	frames = frames + 1
	if time > 1.0 then
		GUI.setString(fpsCounter, "FPS: " .. frames)
		frames = 0
		time = 0
	end
	--Move camera with standard FPS controls
	local mousex, mousey = input.getMousePos()
	mousex = mousex - (width/2)
	mousey = mousey - (height/2)
	camera.turn(cam, mousex*sensitvity*dt, 
		mousey*sensitvity*dt)
	input.setMousePos(width/2, height/2)

	if input.isKeyDown(keys.W) then 
		camera.move(cam, speed*dt)
	end 
	if input.isKeyDown(keys.S) then
		camera.move(cam, -speed*dt)
	end
	if input.isKeyDown(keys.A) then
		camera.strafe(cam, -speed*dt)
	end
	if input.isKeyDown(keys.D) then
		camera.strafe(cam, speed*dt)
	end
end
