function init()
	fixit = GO.createIQM("mr_fixit.iqm")
end

function update(dt)
	aDown = input.isKeyDown(keys.Up)
	if aDown == true then
		print("A DOWN!")
	end
end
