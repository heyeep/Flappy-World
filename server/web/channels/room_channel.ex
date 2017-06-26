defmodule Server.RoomChannel do
  use Server.Web, :channel
  alias Server.RoomState
  alias Server.PlayerList
  require Logger

  def join("room:" <> room_id, _params, socket) do
    player_list = room_id |> get_or_create_player_list
    new_player = PlayerList.new_player(player_list)
    other_players = PlayerList.get_other_players(player_list, new_player)
    stage = room_id |> get_stage

    resp = %{me: new_player, others: other_players}
    Logger.info("join() resp: #{inspect(resp)}")
    send(self, {:after_join, new_player})

    num_players = PlayerList.get_num_players(player_list)
    required_num_players_per_room = 1
    if num_players >= required_num_players_per_room do
      send(self, {:start, stage})
    end

    {:ok, resp, assign(socket, :room_id, room_id)}
  end

  def handle_info({:after_join, new_player}, socket) do
    # Broadcast a Join event for the new player for those already in the channel.
    broadcast! socket, "new_player_joined", new_player
    {:noreply, socket}
  end

  def handle_info({:start, stage}, socket) do
    broadcast! socket, "start", %{stage: stage}
    {:noreply, socket}
  end

  def handle_in(event, params, socket) do
    nil |>
      handle_in(event, params, socket)
  end

  def handle_in(user, "coordinates", params, socket) do
    player_list = get_or_create_player_list(socket.assigns[:room_id])

    player = PlayerList.get_player(player_list, params["server_id"])
    Logger.debug("handle_in:coordinates params: #{inspect(params)}")
    Logger.debug("handle_in:coordinates player: #{inspect(player)}")

    updated_player = Map.put(player, :x, params["x"])
    |> Map.put(:y, params["y"])
    |> Map.put(:lv_x, params["lv_x"])
    |> Map.put(:lv_y, params["lv_y"])

    PlayerList.update_player(player_list, updated_player)

    # Send back the player with its updated x and y coordinates.
    broadcast! socket, "coordinates", updated_player
    {:noreply, socket}
  end

  defp get_or_create_player_list(room_id) do
    room_state = RoomState.get_or_create_room_state(room_id)
    room_state[:player_list]
  end

  defp get_stage(room_id) do
    room_state = RoomState.get_or_create_room_state(room_id)
    room_state[:stage]
  end
end
