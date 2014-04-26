Peer = {}
Peer.__index = Peer

function Peer.getPeer(list, id)
	for k,v in pairs(list) do
		if v.id  == id then
			return v
		end
	end
	return nil
end
