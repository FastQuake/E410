function onPeerConnect(address, port)
	print("Got connection from: "..address..":"..port.."\n")
end

forward = false
function onReceivePacket(address, port, data)
	print("Got packet type: "..data[1].." from "..
						address..":"..port.."\n")
	if data[1] == "forward" then
		forward = true
	elseif data[1] == "turn" then
		if data[2] == "left" then
			box:turn(-1,0)
		elseif data[2] == "right" then
			box:turn(1, 0)
		end
	end
end

function onPeerDisconnect(address, port)
	print("peer "..address..":"..port.." disconnected\n")
end

function init()
	box = GO.loadIQM("cube.iqm","box")
end

function update(dt)
	if forward then
		box:move(1.0*dt)
		forward = false
	end
end
