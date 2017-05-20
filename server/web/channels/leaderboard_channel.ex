defmodule Server.LeaderBoardChannel do
  use Server.Web, :channel
  require Logger

  def join("leaderboard:" <> room_id, _params, socket) do
    resp = %{me: 1, list: [%{name: "John", score: 20},
                           %{name: "Hiep", score: 50},
                           %{name: "Kevin", score: 30},
                           %{name: "Josh", score: 15}]}
    {:ok, resp, assign(socket, :room_id, room_id)}
  end
end
