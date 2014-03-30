function onPeerConnect(address, port)
	print("Got connection from: "..address..":"..port)
end

function onPeerDisconnect(address, port)
	print("peer "..address..":"..port.." disconnected")
end

function init()
end

function update(dt)
end
