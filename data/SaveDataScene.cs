using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using System.Threading.Tasks;
using System.Globalization;

public class SaveDataScene : MonoBehaviour {


	void Start ()
	{
		//GameObject go = GameObject.Find("Main Camera");
		// angle = (angle > 180) ? angle - 360 : angle;
		int num_lights = 0;
		Camera cam = GetComponent<Camera>();
		GameObject[] rootObjects = UnityEngine.SceneManagement.SceneManager.GetActiveScene().GetRootGameObjects();

		string path = Application.dataPath + "/SaveDataScene/data.txt";
		StreamWriter sw = File.CreateText(path);
			for (int i = 1; i < rootObjects.Length-num_lights; ++i)
			{
				WriteObject(rootObjects[i],sw);
				List<GameObject> children = new List<GameObject>();
				children = GetChildren(rootObjects[i], children);
				for (int j = 0; j < children.Count; j++) {
					Debug.Log(children[j]);
					WriteObject(children[j],sw);
				}
	
			}


		//Output the Game data path to the console
		//Debug.Log(path);
	}

	List<GameObject> GetChildren(GameObject go, List<GameObject> children)
	{

		foreach (Transform tran in go.transform)
		{
			children.Add(tran.gameObject);
			children = GetChildren(tran.gameObject, children);
		}
		return children;
	}


	void WriteObject(GameObject go, StreamWriter sw)
	{

		String[] ObjName = go.name.Split(' ');
			float anglex = go.transform.eulerAngles.x;
			anglex = (anglex > 180) ? anglex - 360 : anglex;
			float angley = go.transform.eulerAngles.y;
			angley = (angley > 180) ? angley - 360 : angley;
			float anglez = go.transform.eulerAngles.z;
			anglez = (anglez > 180) ? anglez - 360 : anglez;
			//Vector3 pos = cam.WorldToScreenPoint(rootObjects[i].transform.localPosition);
			//Debug.Log(ObjName[0] + "Position : " + rootObjects[i].transform.localPosition + rootObjects[i].transform.localRotation.eulerAngles.ToString("0.000"));
			sw.WriteLine(ObjName[0] + ".obj " +
				go.transform.position.x.ToString("0.000", CultureInfo.InvariantCulture) + "," +
				go.transform.position.y.ToString("0.000", CultureInfo.InvariantCulture) + "," +
				go.transform.position.z.ToString("0.000", CultureInfo.InvariantCulture) + " " +
				anglex.ToString("0.000", CultureInfo.InvariantCulture) + "," +
				angley.ToString("0.000", CultureInfo.InvariantCulture) + "," +
				anglez.ToString("0.000", CultureInfo.InvariantCulture) + " " +
				//rootObjects[i].transform.rotation.x.ToString("0.000", CultureInfo.InvariantCulture) + "," +
				//rootObjects[i].transform.rotation.y.ToString("0.000", CultureInfo.InvariantCulture) + "," +
				//rootObjects[i].transform.rotation.z.ToString("0.000", CultureInfo.InvariantCulture) + " " +
				//rootObjects[i].transform.localEulerAngles.x.ToString("0.000", CultureInfo.InvariantCulture) + "," +
				//rootObjects[i].transform.localEulerAngles.y.ToString("0.000", CultureInfo.InvariantCulture) + "," +
				//rootObjects[i].transform.localEulerAngles.z.ToString("0.000", CultureInfo.InvariantCulture) + " " +
				go.transform.lossyScale.x.ToString("0.000", CultureInfo.InvariantCulture) + "," +
				go.transform.lossyScale.y.ToString("0.000", CultureInfo.InvariantCulture) + "," +
				go.transform.lossyScale.z.ToString("0.000", CultureInfo.InvariantCulture));
		
	}
}