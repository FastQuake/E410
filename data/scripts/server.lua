function onPeerConnect(address, port)
	print("Got connection from: "..address..":"..port.."\n")
end

function onReceivePacket(address, port, data)
	print("Got packet type: "..data[1].." from "..
						address..":"..port.."\n")
end

function onPeerDisconnect(address, port)
	print("peer "..address..":"..port.." disconnected\n")
end

function init()
	box = GO.loadIQM("cube.iqm","box")
end

function update(dt)
end
