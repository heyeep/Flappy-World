defmodule Server.GameChannel do
  use Server.Web, :channel
  alias Server.GameState
  alias Server.PlayerList
  require Logger

  def join("game:" <> game_id, _params, socket) do
    player_list = game_id |> get_or_create_player_list
    new_player = PlayerList.new_player(player_list)
    other_players = PlayerList.get_other_players(player_list, new_player)
    stage = game_id |> get_stage

    resp = %{me: new_player, others: other_players}
    Logger.info("join() resp: #{inspect(resp)}")
    send(self, {:after_join, new_player})

    num_players = PlayerList.get_num_players(player_list)
    required_num_players_per_game = 1
    if num_players >= required_num_players_per_game do
      send(self, {:start, stage})
    end

    {:ok, resp, assign(socket, :game_id, game_id)}
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
    player_list = get_or_create_player_list(socket.assigns[:game_id])

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

  defp get_or_create_player_list(game_id) do
    game_state = GameState.get_or_create_game_state(game_id)
    game_state[:player_list]
  end

  defp get_stage(game_id) do
    game_state = GameState.get_or_create_game_state(game_id)
    game_state[:stage]
  end
end
