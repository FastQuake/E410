Peer = {}
Peer.__index = Peer

function Peer.create(address, port)
	local peer = {}
	setmetatable(peer,Peer)

	peer.address = address
	peer.port = port

	return peer
end

function Peer.__eq(lhs, rhs)
	if lhs.peer == rhs.peer and lhs.port == rhs.port then
		return true
	else
		return false
	end
end

function Peer.getPeer(list, peer)
	for k,v in pairs(list) do
		if v == peer then
			return v
		end
	end
	return nil
end
