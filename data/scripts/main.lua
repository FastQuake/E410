function init()
	fixit = GO.createIQM("mr_fixit.iqm")
	floor = GO.createIQM("cube.iqm")
	shit = GO.createIQM("monkey.iqm")

	GO.setScale(floor,10,0.1,10)
	GO.setPos(floor,0,-0.5,0)
	GO.setPos(shit,5,1.1,0)
end

function update(dt)
	down = input.isGuiMouseDown(mouse.Left)
	if down == true then
		print("DOWN DOWN DOWN")
	end
end
