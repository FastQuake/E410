function onPeerConnect(address, port)
	print("Got connection from: "..address..":"..port.."\n")
end

x = 0
forward = false
function onReceivePacket(address, port, data)
	print("Got packet type: "..data[1].." from "..
						address..":"..port.."\n")
	if data[1] == "forward" then
		forward = true
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
		x = x + 1*dt
		box:setPos(x, 0, 0)
		forward = false
	end
end
