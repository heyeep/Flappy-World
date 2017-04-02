defmodule Server.RoomState do
  use GenServer
  alias Server.PlayerList

  @doc """
  Used by the supervisor to start the GenServer that will keep the game state
  persistent.

  The initial value passed to the GenServer is an empty map.
  """
  def start_link(room_states) do
    GenServer.start_link(__MODULE__, room_states, name: __MODULE__)
  end

  @doc """
  Return RoomState associated with room_id creating it if it doesn't exist.
  """
  def get_or_create_room_state(room_id) do
    GenServer.call(__MODULE__, {:get_room_state, room_id})
  end

  def handle_call({:get_room_state, room_id}, _from, room_states) do
    case Map.has_key?(room_states, room_id) do
      true ->
        {:reply, room_states[room_id], room_states}
      _ ->
        {:ok, player_list} = PlayerList.start_link(%{})
        room_state = %{player_list: player_list}
        new_room_states = Map.put(room_states, room_id, room_state)
        {:reply, room_state, new_room_states}
    end
  end
end
