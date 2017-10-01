defmodule Server.GameState do
  use GenServer
  alias Server.PlayerList
  alias Server.Stage

  @doc """
  Used by the supervisor to start the GenServer that will keep the game state
  persistent.

  The initial value passed to the GenServer is an empty map.
  """
  def start_link(game_states) do
    GenServer.start_link(__MODULE__, game_states, name: __MODULE__)
  end

  @doc """
  Return GameState associated with game_id creating it if it doesn't exist.
  """
  def get_or_create_game_state(game_id) do
    GenServer.call(__MODULE__, {:get_game_state, game_id})
  end

  def handle_call({:get_game_state, game_id}, _from, game_states) do
    case Map.has_key?(game_states, game_id) do
      true ->
        {:reply, game_states[game_id], game_states}
      _ ->
        {:ok, player_list} = PlayerList.start_link(%{})
        stage = %{pipes: [%{type: "top",    x: 1500.0, y: 200.0},
                          %{type: "bottom", x: 1500.0, y:   0.0},
                          %{type: "top",    x: 1900.0, y: 300.0},
                          %{type: "bottom", x: 1900.0, y: 200.0}]}
        game_state = %{player_list: player_list, game_id: game_id, stage: stage}
        new_game_states = Map.put(game_states, game_id, game_state)
        {:reply, game_state, new_game_states}
    end
  end
end
