import axios from "axios"

export const getGames = async (setGames, setPath, index) => {
    try {
        const response = await axios.get("http://localhost:3001/getGames", { params: { esrb: index }});

        setGames(() => response.data.games)
        setPath(() => response.data.path)

        console.log(response)
      } catch (error) {
        console.log(error)
    }
}

export const launchGame = async (path, antiMicroPath) => {
  try {
    await axios.post("http://localhost:3001/executeShortcut", {
      path: path,
      antiMicroPath: antiMicroPath
    });
  } catch (error) {
    console.log(error)
  }
}