require "title"
sensitvity = 1
speed = 10

function createObject(obj)
end

function init()
	title = Title.create()
	cam = camera.createCam()
	cam:setPos(0,0,0)
	cam:turn(0,90)
	camera.setCam(cam)
end

function update(dt)
	title:update(dt)
end
