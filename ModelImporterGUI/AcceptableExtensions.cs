using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModelImporterGUI
{
    public class AcceptableExtensions
    {
        public static void InitializeAcceptableExtensions()
        {
            acceptedExtensions = new List<string>();

            acceptedExtensions.Add(".fbx");
            acceptedExtensions.Add(".dae");
            acceptedExtensions.Add(".gltf");
            acceptedExtensions.Add(".glb");
            acceptedExtensions.Add(".blend");
            acceptedExtensions.Add(".3ds");
            acceptedExtensions.Add(".ase");
            acceptedExtensions.Add(".obj");
            acceptedExtensions.Add(".ifc");
            acceptedExtensions.Add(".xgl");
            acceptedExtensions.Add(".zgl");
            acceptedExtensions.Add(".ply");
            acceptedExtensions.Add(".dxf");
            acceptedExtensions.Add(".lwo");
            acceptedExtensions.Add(".lws");
            acceptedExtensions.Add(".lxo");
            acceptedExtensions.Add(".stl");
            acceptedExtensions.Add(".x");
            acceptedExtensions.Add(".ac");
            acceptedExtensions.Add(".ms3d");
            acceptedExtensions.Add(".cob");
            acceptedExtensions.Add(".scn");
            acceptedExtensions.Add(".bvh");
            acceptedExtensions.Add(".csm");
            acceptedExtensions.Add(".xml");
            acceptedExtensions.Add(".irrmesh");
            acceptedExtensions.Add(".irr");
            acceptedExtensions.Add(".mdl");
            acceptedExtensions.Add(".md2");
            acceptedExtensions.Add(".md3");
            acceptedExtensions.Add(".pk3");
            acceptedExtensions.Add(".mdc");
            acceptedExtensions.Add(".md5");
            acceptedExtensions.Add(".smd");
            acceptedExtensions.Add(".vta");
            acceptedExtensions.Add(".ogex");
            acceptedExtensions.Add(".3d");
            acceptedExtensions.Add(".b3d");
            acceptedExtensions.Add(".q3d");
            acceptedExtensions.Add(".q3s");
            acceptedExtensions.Add(".nff");
            acceptedExtensions.Add(".off");
            acceptedExtensions.Add(".raw");
            acceptedExtensions.Add(".ter");
            acceptedExtensions.Add(".hmp");
            acceptedExtensions.Add(".ndo");
        }

        public static List<string> acceptedExtensions { get; set; }
    }
}
