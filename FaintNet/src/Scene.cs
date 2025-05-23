﻿using Coral.Managed.Interop;
using Faint.Net;
using System;
using System.Reflection.Metadata;

namespace Faint.Net
{
    public class Scene
    {
        internal static unsafe delegate*<NativeString, int> GetEntityIcall;
        internal static unsafe delegate*<NativeString, NativeInstance<Entity>> GetEntityScriptFromNameIcall;
        internal static unsafe delegate*<int, NativeInstance<Entity>> GetEntityScriptFromHandleIcall;
        internal static unsafe delegate*<NativeString, int> InstancePrefabIcall;

        public static T? GetEntity<T>(string entityName) where T : class
        {
            NativeInstance<Entity> handle;
            unsafe { handle = GetEntityScriptFromNameIcall(entityName); }

            Entity? entity = handle.Get();
            if (entity != null && entity is T)
            {
                return entity as T;
            }

            return null;
        }

        public static T? GetEntity<T>(int entityHandle) where T : class
        {
            NativeInstance<Entity> handle;
            unsafe { handle = GetEntityScriptFromHandleIcall(entityHandle); }

            Entity? entity = handle.Get();
            if (entity != null && entity is T)
            {
                return entity as T;
            }

            return null;
        }

        public static Entity GetEntity(string entityName)
        {
            int handle;
            unsafe { handle = GetEntityIcall(entityName); }

            if (handle == -1)
            {
                throw new Exception("Entity not found");
            }

            Entity entity = new Entity(handle);

            return entity;
        }

        public static T? InstancePrefab<T>(string path, Entity? parent = null) where T : class
        {
            int handle;
            unsafe { handle = InstancePrefabIcall(path); }

            if (handle == -1)
            {
                return null;
            }

            //NativeInstance<Entity> nativeInstance;
            //unsafe { nativeInstance = GetEntityScriptFromHandleIcall(handle); }
            //
            //Entity? entity = nativeInstance.Get();
            //if (entity != null && entity is T)
            //{
            //    return entity as T;
            //}


            NativeInstance<Entity> nativeInstance;
            unsafe { nativeInstance = GetEntityScriptFromHandleIcall(handle); }

            Entity? entity = nativeInstance.Get();
            if (entity != null && entity is T)
            {
                return entity as T;
            }

            return null;
        }
    }
}
