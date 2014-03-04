function init()
	fixit = GO.createIQM("mr_fixit.iqm")
end

function update(dt)
	down = input.isGuiMouseDown(mouse.Left)
	if down == true then
		print("DOWN DOWN DOWN")
	end
end
