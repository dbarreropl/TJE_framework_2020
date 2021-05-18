using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Threading.Tasks;
using System.Globalization;


public class SaveDataScene : MonoBehaviour {

	void Start ()
	{
		//GameObject go = GameObject.Find("Main Camera");
		// angle = (angle > 180) ? angle - 360 : angle;
		int num_lights = 1;
		Camera cam = GetComponent<Camera>();
		GameObject[] rootObjects = UnityEngine.SceneManagement.SceneManager.GetActiveScene().GetRootGameObjects();

		string path = Application.dataPath + "/SaveDataScene/data.txt";
		using (StreamWriter sw = File.CreateText(path))
		{
			for (int i = 1; i < rootObjects.Length-num_lights; ++i)
			{
				String[] ObjName = rootObjects[i].name.Split(' ');
				float anglex = rootObjects[i].transform.localEulerAngles.x;
				anglex = (anglex > 180) ? anglex - 360 : anglex;
				float angley = rootObjects[i].transform.localEulerAngles.y;
				angley = (angley > 180) ? angley - 360 : angley;
				float anglez = rootObjects[i].transform.localEulerAngles.z;
				anglez = (anglez > 180) ? anglez - 360 : anglez;
				//Vector3 pos = cam.WorldToScreenPoint(rootObjects[i].transform.localPosition);
				Debug.Log(ObjName[0] + "Position : " + rootObjects[i].transform.localPosition + rootObjects[i].transform.localRotation.eulerAngles.ToString("0.000"));
				sw.WriteLine(ObjName[0] + ".obj " +
					rootObjects[i].transform.localPosition.x.ToString("0.000", CultureInfo.InvariantCulture) + "," +
					rootObjects[i].transform.localPosition.y.ToString("0.000", CultureInfo.InvariantCulture) + "," +
					rootObjects[i].transform.localPosition.z.ToString("0.000", CultureInfo.InvariantCulture) + " " +
					anglex.ToString("0.000", CultureInfo.InvariantCulture) + "," +
					angley.ToString("0.000", CultureInfo.InvariantCulture) + "," +
					anglez.ToString("0.000", CultureInfo.InvariantCulture) + " " +
					//rootObjects[i].transform.rotation.x.ToString("0.000", CultureInfo.InvariantCulture) + "," +
					//rootObjects[i].transform.rotation.y.ToString("0.000", CultureInfo.InvariantCulture) + "," +
					//rootObjects[i].transform.rotation.z.ToString("0.000", CultureInfo.InvariantCulture) + " " +
					//rootObjects[i].transform.localEulerAngles.x.ToString("0.000", CultureInfo.InvariantCulture) + "," +
					//rootObjects[i].transform.localEulerAngles.y.ToString("0.000", CultureInfo.InvariantCulture) + "," +
					//rootObjects[i].transform.localEulerAngles.z.ToString("0.000", CultureInfo.InvariantCulture) + " " +
					rootObjects[i].transform.localScale.x.ToString("0.000", CultureInfo.InvariantCulture) + "," +
					rootObjects[i].transform.localScale.y.ToString("0.000", CultureInfo.InvariantCulture) + "," +
					rootObjects[i].transform.localScale.z.ToString("0.000", CultureInfo.InvariantCulture));
			}
		}

		//Output the Game data path to the console
		Debug.Log(path);
	}
}